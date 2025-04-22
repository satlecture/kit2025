/*************************************************************************************************
StreamBuffer -- Copyright (c) 2021, Markus Iser, KIT - Karlsruhe Institute of Technology

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and
associated documentation files (the "Software"), to deal in the Software without restriction,
including without limitation the rights to use, copy, modify, merge, publish, distribute,
sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or
substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT
NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND
NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM,
DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT
OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 **************************************************************************************************/

#ifndef STREAMBUFFER_H_
#define STREAMBUFFER_H_

#include <archive.h>
#include <archive_entry.h>

#include <limits>
#include <cstring>

#include "SolverTypes.h"

class ParserException : public std::exception {
 public:
    explicit ParserException(const std::string& what) noexcept : m_what(what) { }
    const char* what() const noexcept {
        return m_what.c_str();
    }

 private:
    std::string m_what;
};

class StreamBuffer {
    struct archive* file;

    unsigned int buffer_size;
    char* buffer;

    unsigned int pos;  // current read position
    unsigned int end;  // 1+last valid position
    bool end_of_file;  // true when last chunk of file was read to buffer

    const char* filename_;

    bool refill_buffer(bool align = true) {
        if (pos >= end && !end_of_file) {
            pos = 0;
            if (end > 0 && end < buffer_size) {
                std::copy(buffer + end, buffer + buffer_size, buffer);
                end = buffer_size - end;
            } else {
                end = 0;
            }
            end += archive_read_data(file, buffer + end, buffer_size - end);
            if (end < buffer_size) {
                std::memset(buffer + end, 0, buffer_size - end);
                end_of_file = true;
            } else {
                if (align) align_buffer();
            }
            // In rare cases, the last buffer might have exactly lined up with
            // eof, leading to `pos = end = 0` and `end_of_file = true`, in
            // which case we want to return that eof was reached.
            return end > 0;
        }
        return false;
    }
    
    void align_buffer() {
        while (!isspace(buffer[end-1])) {  // align buffer with word-end
            end--;
            if (end < 1) {
                throw ParserException(std::string("Error reading file: maximum token length exceeded"));
            }
        }
    }

 public:
    explicit StreamBuffer(const char* filename) : buffer_size(16384), pos(0), end(0), end_of_file(false), filename_(filename) {
        file = archive_read_new();
        archive_read_support_filter_all(file);
        // archive_read_support_format_raw(file);
        archive_read_support_format_raw(file);
        int r = archive_read_open_filename(file, filename, buffer_size);
        if (r != ARCHIVE_OK) {
            throw ParserException(std::string(archive_error_string(file)) + std::string(" Error opening file: ") + std::string(filename));
        }
        struct archive_entry *entry;
        r = archive_read_next_header(file, &entry);
        if (r != ARCHIVE_OK) {
            throw ParserException(std::string("Error reading header: ") + std::string(filename));
        }
        buffer = new char[buffer_size];
        refill_buffer();
    }

    ~StreamBuffer() {
        archive_read_free(file);
        delete[] buffer;
    }

    char operator *() const {
        return eof() ? EOF : buffer[pos];
    }

    /**
     * @brief check if eof reached
     * @return true if eof reached, false otherwise
     */
    bool eof() const {
        return (pos >= end) && end_of_file;
    }

    /**
     * @brief skip one character, return false if eof reached
     * @pre !eof()
     * @return true if pos valid, false otherwise (eof reached)
     */
    bool skip(bool align = true) {
        // if (eof()) return false;
        if (++pos < end) {
            return true;
        } else {
            return refill_buffer(align);
        }
    }

    /**
     * @brief skip entire line and subsequent whitespace, return false if eof reached
     * @pre !eof()
     * @return true if pos is valid, false otherwise (eof reached)
     */
    bool skipLine() {
        // if (eof()) return false;
        while (buffer[pos] != '\n' && buffer[pos] != '\r') {
            if (!skip(false)) return false;
        }
        // manually align buffer after line is skipped to be able to skip lines
        // with words longer than the stream buffer
        align_buffer();
        return skipWhitespace();
    }

    /**
     * @brief skip whitespace, return false if eof reached
     * @pre !eof()
     * @return true if pos is valid, false otherwise (eof reached)
     */
    bool skipWhitespace() {
        // needed if last call to fill_buffer left pos == end == 0
        if (eof()) return false;
        while (isspace(buffer[pos])) {
            if (!skip()) return false;
        }
        return true;
    }

    /**
     * @brief skip given string
     * @param str the string to match
     * @throw if str could not be entirely matched
     * @return true if pos is valid, false otherwise (eof reached)
     */
    bool skipString(const char* str) {
        if (eof()) return false;
        while (*str == buffer[pos]) {
            ++str;
            if (*str == '\0') {
                return skip();
            }
            if (!skip()) {
                throw ParserException(std::string(filename_) + ": expected '" + str + "'");
            }
        }
        throw ParserException(std::string(filename_) + ": expected '" + str + "'");
    }

    /**
     * @brief skip next number, skip leading whitespace
     * @throw ParserException if no number could be read
     * @return true if number was read before reaching eof, false otherwise
     */
    bool skipNumber() {
        if (!skipWhitespace()) return false;

        if (buffer[pos] == '-') {
            if (!skip()) return false;
        } else if (buffer[pos] == '+') {
            if (!skip()) return false;
        }

        if (!isdigit(buffer[pos])) {
            if (!skipWhitespace()) return false;
            if (!isdigit(buffer[pos])) {
                throw ParserException(std::string(filename_) + ": unexpected character: " + buffer[pos]);
            }
        }

        while (isdigit(buffer[pos])) {
            if (!skip()) break;
        }
        
        return true;
    }

    /**
     * @brief read next integer, skip leading whitespace
     * @param *out the read integer, output parameter
     * @throw ParserException if no integer could be read
     * @return true if integer was read before reaching eof, false otherwise
     */
    bool readInteger(int* out) {
        if (!skipWhitespace()) return false;

        char* str = buffer + pos;
        char* end = nullptr;

        errno = 0;
        long number = strtol(str, &end, 10);

        if (errno == 0) {
            if (end > str) {
                if (std::abs(number) <= std::numeric_limits<int32_t>::max()) {
                    pos += static_cast<intptr_t>(end - str);
                    *out = static_cast<int>(number);
                    return true;
                } else {
                    throw ParserException(std::string(filename_) + ": number out of int32 range");
                }
            } else {
                throw ParserException(std::string(filename_) + ": unexpected character: " + buffer[pos]);
            }
        } else {
            throw ParserException(std::string(filename_) + ": strtol() errno: " + std::to_string(errno));
        }
    }

    /**
     * @brief read next unsigned 64bit integer, skip leading whitespace
     * @param *out the read integer, output parameter
     * @throw ParserException if no integer could be read
     * @return true if integer was read before reaching eof, false otherwise
     */
    bool readUInt64(uint64_t* out) {
        if (!skipWhitespace()) return false;

        char* str = buffer + pos;
        char* end = nullptr;

        errno = 0;
        unsigned long long number = strtoull(str, &end, 10);

        if (errno == 0) {
            if (end > str) {
                if (number <= std::numeric_limits<uint64_t>::max()) {
                    pos += static_cast<intptr_t>(end - str);
                    *out = static_cast<uint64_t>(number);
                    return true;
                } else {
                    throw ParserException(std::string(filename_) + ": number out of uint64 range");
                }
            } else {
                throw ParserException(std::string(filename_) + ": unexpected character: " + buffer[pos]);
            }
        } else {
            throw ParserException(std::string(filename_) + ": strtoull() errno: " + std::to_string(errno));
        }
    }

    /**
     * @brief read next number, skip leading whitespace
     * @param *out  the read number, output parameter
     * @throw ParserException if no number could be read
     * @return true if number was read before reaching eof, false otherwise
     */
    bool readNumber(std::string* out) {
        std::string result;

        if (!skipWhitespace()) return false;

        if (buffer[pos] == '-') {
            result.append(1, buffer[pos]);
            if (!skip()) return false;
        } else if (buffer[pos] == '+') {
            if (!skip()) return false;
        }

        if (!isdigit(buffer[pos])) {
            if (!skipWhitespace()) return false;
            if (!isdigit(buffer[pos])) {
                throw ParserException(std::string(filename_) + ": unexpected character: " + buffer[pos]);
            }
        }

        while (isdigit(buffer[pos])) {
            result.append(1, buffer[pos]);
            if (!skip()) break;
        }
        
        *out = result;
        return true;
    }

    /**
     * @brief read next clause
     * @param out the read clause, output parameter
     * @return true if clause was read before reaching eof, false otherwise
     */
    bool readClause(Cl& out) {
        Cl clause;

        if (eof() || !skipWhitespace()) return false;

        while (buffer[pos] == 'p' || buffer[pos] == 'c') {
            if (!skipLine()) return false;
        }

        int plit;
        while (readInteger(&plit)) {
            if (plit == 0) break;
            clause.push_back(Lit(abs(plit), plit < 0));
        }
        
        out = clause;
        return true;
    }
};

#endif  // STREAMBUFFER_H_
