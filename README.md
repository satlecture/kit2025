# Lecture: Practical SAT Solving

_Summer Term 2025 • [GitHub repository](https://github.com/satlecture/kit2025) • [Web page](https://satlecture.github.io/kit2025/) • Karlsruhe Institute of Technology (KIT)_


This lecture with exercises is offered by [Ashlin Iser (ITI Sanders)](https://ae.iti.kit.edu/english/3986.php) and [Dominik Schreiber (KASTEL-VADS SAtRes)](https://satres.kikit.kit.edu/schreiber).
[Niccolò Rigi-Luperti (SAtRes)](https://satres.kikit.kit.edu/team/) acts as a co-manager of exercises.  
The lecture provides a friendly and practical overview of the subject of propositional satisfiability (SAT) solving, including its theoretical background, important algorithms and techniques, parallelization, proofs, applications, and related tools (MaxSAT, SMT).  
No particular prior knowledge beyond basic foundations of computer science is required.

All lectures and exercises take place at **15:45 in building 50.34 room 301**.
Our first appointment is the lecture on **Tuesday, April 22**.

## Exam Dates
We tentatively offer the following exam dates (last updated July 23):

* ~~Thursday, July 31~~
* Monday, August 25
* Wednesday, September 24

If neither of these options suit you, please let us know!

## Lectures

The below lecture plan is tentative and subject to changes.
We try to link each slide set here shortly before the lecture takes place.
See also [the page from last year](https://github.com/satlecture/kit2024).

* **Tuesday, April 22**: [L1 - Organisation, Introduction, Applications, Encodings, IPASIR](slides/l01-introduction.pdf) _[MI+DS]_
* **Monday, April 28**: [L2 - Tractable Subclasses, Elementary Encoding Methods](slides/l02-encodings.pdf) _[MI]_
* **Monday, May 5**: [L3 - Local Search, Resolution, DP Algorithm, DPLL Algorithm](slides/l03-sls-dpll.pdf) _[DS]_
* **Monday, May 12**: [L4 - Branching Heuristics, Restart Strategies, Clause Learning, Backtracking](slides/l04-heuristics-cdcl.pdf) _[MI]_
* **Monday, May 19**: [L5 - Efficient Unit Propagation, Clause Forgetting, Community Structure, VSIDS](slides/l05-cdcl.pdf) _[MI]_
* **Monday, May 26**: [L6 - Preprocessing and Propagation-based Redundancy](slides/l06-preprocessing.pdf) _[MI]_
* **Monday, June 2**: [L7 - Redundancy and Proof Systems](slides/l07-redundancy-proofs.pdf) _[MI]_
* **Monday, June 16**: [L8.1 - Parallel SAT Solving: Search space partitioning, Portfolios, Clause sharing, massively parallel basics](slides/l08-parallel.pdf) _[DS]_
* **Monday, June 23**: [L8.2 - Frontiers of distributed SAT solving](slides/l08-parallel.pdf) _[DS]_
* **Monday, June 30**: [L9.1 - Pragmatics of proofs of unsatisfiability](slides/l09-proof-pragmatics-parallel.pdf) _[DS]_
* **Monday, July 7**: [L9.2 - Parallel proof & distributed technology](slides/l09-proof-pragmatics-parallel.pdf) ; [L10 - Applications 1: Automated planning](slides/l10-planning-model-checking.pdf) _[DS]_
* **Monday, July 14**: [L11 - Selected application highlights](slides/l11-application-highlights.pdf) _[DS]_
* **Monday, July 21**: [L12 - Satisfiability Modulo Theories (SMT) solving](slides/l12-smt.pdf) _[DS]_
* **Monday, July 28**: [L13 - Maximum Satisfiability (MaxSAT) solving](slides/l13-maxsat.pdf) _[MI]_

## Exercises

This is our tentative plan for exercises:

* **Tuesday, May 6**: E0 - getting started, announcement of [assignment 1 (A1) (introduction, algorithms, encodings)](exercises/ex1.pdf) _[DS]_
* **Tuesday, May 20**: E1 - discussion of A1, announcement of [A2 (classic and modern SAT solving)](exercises/ex2.pdf) _[MI]_
* **Tuesday, June 3**: E2 - discussion of A2, announcement of [A3 (preprocessing, proofs)](exercises/ex3.pdf) _[MI]_
* **Tuesday, June 17**: E3 - discussion of A3, announcement of [A4 (parallel SAT)](exercises/ex4.pdf) _[MI]_
* **Tuesday, July 1**: E4 - discussion of A4, announcement of [A5 (proof pragmatics, planning)](exercises/ex5.pdf) _[DS]_
* **Tuesday, July 15**: E5 - discussion of A5, announcement of [A6 (applications, SMT)](exercises/ex6.pdf) _[DS]_
* **Tuesday, July 29**: E6 - discussion of A6, exam questions

## Code

* **Tuesday, April 22**: [Essential Variables](code/src/essential.cc)
