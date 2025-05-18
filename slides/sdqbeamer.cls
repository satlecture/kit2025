%% Vorlage für Präsentationen mit LaTeX Beamer im KIT-Design
%% entsprechend den Gestaltungsrichtlinien vom Februar 2025
%%
%% Siehe https://sdq.kastel.kit.edu/wiki/Dokumentvorlagen


\NeedsTeXFormat{LaTeX2e}
\ProvidesClass{sdqbeamer}[2025-03-01 v4.0.0 SDQ Beamer class]

\RequirePackage[utf8]{inputenc}
\RequirePackage[T1]{fontenc}

\newif\ifsectionnavigationoption
\newif\ifsectionnavigation
\sectionnavigationoptiontrue
\newif\ifgerman
\newif\iffranklin
\newif\ifsmallfooterfont
\newif\ifkitgrid

\def\groupname#1{\def\@groupname{#1}}
\def\groupnamewidth#1{\def\@groupnamewidth{#1}}

% siehe README.md
\DeclareOption{de}{
  \germantrue
  \PassOptionsToPackage{autostyle}{csquotes}
  }
\DeclareOption{en}{
  \germanfalse
  }
\DeclareOption{navbaroff}{
  \sectionnavigationoptionfalse
}
\DeclareOption{smallfoot}{
  \smallfooterfonttrue
}
\DeclareOption{bigfoot}{
  \smallfooterfontfalse
}
\DeclareOption{helvet}{
  \franklinfalse
}
\DeclareOption{franklin}{
  \franklintrue
}
\DeclareOption{kitgrid}{
  \kitgridtrue
}

\ExecuteOptions{de,bigfoot,franklin}

\DeclareOption*{\PassOptionsToClass{\CurrentOption}{beamer}}

\ProcessOptions\relax

% In der PowerPoint-Vorlage ist die Schrift 16 pt groß. Dies wird von LaTeX aber standardmäßig nicht angeboten.
% Daher wurden 17 pt als Standardschriftgröße gewählt. Das ergibt:
%
% \tiny         = 10pt
% \scriptsize   = 11pt
% \footnotesize = 12pt
% \small        = 14pt
% \normalsize   = 17pt
% \large        = 20pt
% \Large        = 25pt
% \LARGE        = 30pt
% \huge         = 36pt
% \Huge         = 48pt
% \HUGE         = 60pt 
%
% onlytextwidth läßt Spalten nur innerhalb der Ränder erscheinen.
\LoadClass[17pt,utf8,onlytextwidth]{beamer}

% Babel-Paket wird nur bei deutscher Sprache benötigt
\ifgerman
  \RequirePackage[ngerman]{babel}
\fi
\RequirePackage{csquotes}
\RequirePackage{hyperref}
\RequirePackage[absolute,overlay]{textpos}

%% ┏━━━━━━━━━━━━━┓
%% ┃ Typographie ┃
%% ┗━━━━━━━━━━━━━┛

\RequirePackage{microtype}

\RequirePackage{helvet}
\RequirePackage[scaled=.92]{beramono}
\RequirePackage{libertineRoman}

% Semibold
\DeclareRobustCommand{\sbseries}{\fontseries{sb}\selectfont}
\DeclareTextFontCommand{\textsb}{\sbseries}

% Überschriften in Franklin (nur wenn Paket-Option gesetzt, sonst auch Helvetica)
\iffranklin
  \setbeamerfont{title}{family*=LibreFranklin-TLF,series=\sbseries,size=\Huge}
  \setbeamerfont{frametitle}{family*=LibreFranklin-TLF,series=\sbseries,size=\huge}
  \setbeamerfont{framesubtitle}{family*=LibreFranklin-TLF,series=\sbseries,size=\LARGE}
  \setbeamerfont{section in toc}{family*=LibreFranklin-TLF}
\else
  \setbeamerfont{title}{series=\bfseries,size=\Huge}
  \setbeamerfont{frametitle}{series=\bfseries,size=\huge}
  \setbeamerfont{framesubtitle}{series=\bfseries,size=\LARGE}
\fi

\setbeamerfont{subtitle}{family*=phv,series=\bfseries,size=\normalsize}
\setbeamerfont{author}{family*=phv,series=\mdseries,size=\normalsize}
\setbeamerfont{date}{family*=phv,series=\mdseries,size=\normalsize}

% Aufzählungen
\setbeamertemplate{itemize items}[square]

% Boxen
\setbeamerfont{highlight box}{series=\bfseries, size=\normalsize}
\setbeamerfont{standard box}{series=\bfseries, size=\normalsize}


%% ┏━━━━━━━━━━━━━━┓
%% ┃ /Typographie ┃
%% ┗━━━━━━━━━━━━━━┛

%% ┏━━━━━━━━━━━━━━━┓
%% ┃ Folien-Layout ┃
%% ┗━━━━━━━━━━━━━━━┛

% Seitenverhältnis
%
% Die Folien benutzen dieselbe Geometrie wie die PowerPoint-Vorlagen des KIT.

\RequirePackage{geometry}
\geometry{papersize={33.867cm,19.05cm}}

% Ränder laut pptx: 1,03 cm
\newlength{\kitoutermargin}
\setlength{\kitoutermargin}{1.03cm}
% Höhe der Fußzeile laut pptx: 1,62cm
\newlength{\kitbottom}
\setlength{\kitbottom}{1.62cm}
% Abstände im Raster
\newlength{\kitcolumn}
\setlength{\kitcolumn}{4.8cm}
\newlength{\kitcolumnsep}
\setlength{\kitcolumnsep}{0.6cm}
\newlength{\kitrow}
\setlength{\kitrow}{2.8cm}
\newlength{\kitrowsep}
\setlength{\kitrowsep}{0.6cm}

% Position der vertikalen Hilfslinien
\newlength{\kitcolumnoneleft}
\setlength{\kitcolumnoneleft}{\kitoutermargin}
\newlength{\kitcolumnoneright}
\setlength{\kitcolumnoneright}{\dimexpr\kitoutermargin+\kitcolumn\relax}

\newlength{\kitcolumntwoleft}
\setlength{\kitcolumntwoleft}{\dimexpr\kitoutermargin+\kitcolumn+\kitcolumnsep\relax}
\newlength{\kitcolumntworight}
\setlength{\kitcolumntworight}{\dimexpr\kitoutermargin+2\kitcolumn+\kitcolumnsep\relax}

\newlength{\kitcolumnthreeleft}
\setlength{\kitcolumnthreeleft}{\dimexpr\kitoutermargin+2\kitcolumn+2\kitcolumnsep\relax}
\newlength{\kitcolumnthreeright}
\setlength{\kitcolumnthreeright}{\dimexpr\kitoutermargin+3\kitcolumn+2\kitcolumnsep\relax}

\newlength{\kitcolumnfourleft}
\setlength{\kitcolumnfourleft}{\dimexpr\kitoutermargin+3\kitcolumn+3\kitcolumnsep\relax}
\newlength{\kitcolumnfourright}
\setlength{\kitcolumnfourright}{\dimexpr\kitoutermargin+4\kitcolumn+3\kitcolumnsep\relax}

\newlength{\kitcolumnfiveleft}
\setlength{\kitcolumnfiveleft}{\dimexpr\kitoutermargin+4\kitcolumn+4\kitcolumnsep\relax}
\newlength{\kitcolumnfiveright}
\setlength{\kitcolumnfiveright}{\dimexpr\kitoutermargin+5\kitcolumn+4\kitcolumnsep\relax}

\newlength{\kitcolumnsixleft}
\setlength{\kitcolumnsixleft}{\dimexpr\kitoutermargin+5\kitcolumn+5\kitcolumnsep\relax}
\newlength{\kitcolumnsixright}
\setlength{\kitcolumnsixright}{\dimexpr\kitoutermargin+6\kitcolumn+5\kitcolumnsep\relax}

% Position der horizontalen Hilfslinien
\newlength{\kitrowonetop}
\setlength{\kitrowonetop}{\kitoutermargin}
\newlength{\kitrowonebottom}
\setlength{\kitrowonebottom}{\dimexpr\kitoutermargin+\kitrow\relax}

\newlength{\kitrowtwotop}
\setlength{\kitrowtwotop}{\dimexpr\kitoutermargin+\kitrow+\kitrowsep\relax}
\newlength{\kitrowtwobottom}
\setlength{\kitrowtwobottom}{\dimexpr\kitoutermargin+2\kitrow+\kitrowsep\relax}

\newlength{\kitrowthreetop}
\setlength{\kitrowthreetop}{\dimexpr\kitoutermargin+2\kitrow+2\kitrowsep\relax}
\newlength{\kitrowthreebottom}
\setlength{\kitrowthreebottom}{\dimexpr\kitoutermargin+3\kitrow+2\kitrowsep\relax}

\newlength{\kitrowfourtop}
\setlength{\kitrowfourtop}{\dimexpr\kitoutermargin+3\kitrow+3\kitrowsep\relax}
\newlength{\kitrowfourbottom}
\setlength{\kitrowfourbottom}{\dimexpr\kitoutermargin+4\kitrow+3\kitrowsep\relax}

\newlength{\kitrowfivetop}
\setlength{\kitrowfivetop}{\dimexpr\kitoutermargin+4\kitrow+4\kitrowsep\relax}
\newlength{\kitrowfivebottom}
\setlength{\kitrowfivebottom}{\dimexpr\kitoutermargin+5\kitrow+4\kitrowsep\relax}

% Breite von Elementen, die mehrere Spalten umfassen
\newlength{\kitonecolumn}
\setlength{\kitonecolumn}{\kitcolumn}
\newlength{\kittwocolumns}
\setlength{\kittwocolumns}{\dimexpr2\kitcolumn+\kitcolumnsep\relax}
\newlength{\kitthreecolumns}
\setlength{\kitthreecolumns}{\dimexpr3\kitcolumn+2\kitcolumnsep\relax}
\newlength{\kitfourcolumns}
\setlength{\kitfourcolumns}{\dimexpr4\kitcolumn+3\kitcolumnsep\relax}
\newlength{\kitfivecolumns}
\setlength{\kitfivecolumns}{\dimexpr5\kitcolumn+4\kitcolumnsep\relax}
\newlength{\kitsixcolumns}
\setlength{\kitsixcolumns}{\dimexpr6\kitcolumn+5\kitcolumnsep\relax}

% Breite von Elementen, die mehrere Zeilen umfassen
\newlength{\kitonerow}
\setlength{\kitonerow}{\kitrow}
\newlength{\kittworows}
\setlength{\kittworows}{\dimexpr2\kitrow+\kitrowsep\relax}
\newlength{\kitthreerows}
\setlength{\kitthreerows}{\dimexpr3\kitrow+2\kitrowsep\relax}
\newlength{\kitfourrows}
\setlength{\kitfourrows}{\dimexpr4\kitrow+3\kitrowsep\relax}
\newlength{\kitfiverows}
\setlength{\kitfiverows}{\dimexpr5\kitrow+4\kitrowsep\relax}

% Hintergrund-Raster sichtbar
% Nach Vorlagen-pptx
\defbeamertemplate{background canvas}{kitgrid}{
  \begin{pgfpicture}{0pt}{0pt}{\paperwidth}{\paperheight}
    % horizontal
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowonetop}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowonetop}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowonebottom}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowonebottom}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowtwotop}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowtwotop}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowtwobottom}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowtwobottom}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowthreetop}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowthreetop}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowthreebottom}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowthreebottom}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowfourtop}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowfourtop}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowfourbottom}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowfourbottom}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowfivetop}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowfivetop}}
    \pgfpathmoveto{\pgfpoint{0cm}{\paperheight-\kitrowfivebottom}}
    \pgfpathlineto{\pgfpoint{\paperwidth}{\paperheight-\kitrowfivebottom}}
    % vertikal
    \pgfpathmoveto{\pgfpoint{\kitcolumnoneleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnoneleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnoneright}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnoneright}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumntwoleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumntwoleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumntworight}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumntworight}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnthreeleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnthreeleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnthreeright}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnthreeright}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnfourleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnfourleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnfourright}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnfourright}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnfiveleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnfiveleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnfiveright}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnfiveright}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnsixleft}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnsixleft}{\paperheight}}
    \pgfpathmoveto{\pgfpoint{\kitcolumnsixright}{0cm}}
    \pgfpathlineto{\pgfpoint{\kitcolumnsixright}{\paperheight}}
    % zeichnen
    \pgfsetstrokecolor{kit-orange50}
    \pgfusepath{stroke}
  \end{pgfpicture}%
}

% Ränder außen
\setbeamersize{text margin left=\kitoutermargin,text margin right=\kitoutermargin} 

% keine Navigationssymbole
\setbeamertemplate{navigation symbols}{}
\setbeamercovered{invisible}
\beamer@compresstrue % Miniframes (Navigations-Punkte) für Subsections immer in einer Zeile, ohne Umbrüche

% Mehrere Folienlayouts erlauben
% siehe https://stackoverflow.com/questions/3158665/having-multiple-slide-templates-in-latex-beamer

%%
%% Standard-Layout
%%

\BeforeBeginEnvironment{frame}{%
  \restoregeometry
  % Section-Navigation wieder auf Paketoption zurücksetzen
  \ifsectionnavigationoption\sectionnavigationtrue\else\sectionnavigationfalse\fi
  % KIT-Logo bunt
  \footlinelogo{rgb}
  \setbeamertemplate{frametitle}[KIT 2025]{\paperwidth}
  \setbeamertemplate{footline}[KIT 2025]
  \setbeamertemplate{background}[default]
  \setbeamertemplate{logo}{}
  \ifkitgrid\setbeamertemplate{background canvas}[kitgrid]\fi
}

% ┌───────────┐
% │ Kopfzeile │
% └───────────┘

% #1 = Breite
\defbeamertemplate{frametitle}{KIT 2025}[1]{
  % Oberkante Titeltext
  \begin{textblock*}{#1}[0,0](\kitoutermargin,\kitoutermargin)%
    {\usebeamercolor[fg]{frametitle}\usebeamerfont{frametitle}\insertframetitle}%
    \ifx\insertframesubtitle\@empty\else\\\fi
    {\usebeamercolor[fg]{framesubtitle}\usebeamerfont{framesubtitle}\insertframesubtitle}%
  \end{textblock*}%
  % Rand oben entsprechend KIT-Vorlage
  \vspace{37mm}%
}

% #1 = logo
\defbeamertemplate{logo}{KIT 2025 title/toc}[1]{
  \begin{textblock*}{24mm}(297.5mm,\kitoutermargin)
  \includegraphics[width=24mm]{logos/#1} 
  \end{textblock*}
  \vspace{37mm}%
}

% ┌────────────┐
% │ /Kopfzeile │
% └────────────┘

% ┌──────────┐
% │ Fußzeile │
% └──────────┘

% Anpassung der Fußzeile für zweizeilige Titel
\newlength{\kitfooterbaselinecorrection}

% KIT-Logo (rgb, white, black)
\def\footlinelogo#1{\def\@footlinelogo{#1}}

% kleiner Text nur, wenn "smallfoot" gewählt
\ifsmallfooterfont
    % 11pt wie in offiziellen Vorlage
    \setbeamerfont{footer}{size=\scriptsize}
    \setlength{\kitfooterbaselinecorrection}{2pt}
  \else
    % 12pt, besser lesbar
    \setbeamerfont{footer}{size=\footnotesize}
    \setlength{\kitfooterbaselinecorrection}{3pt}
\fi

%
% Section-Punkte, die mit der Schriftgröße skalieren
%
% angepaßt von https://tex.stackexchange.com/questions/292070/beamer-navigation-bar-too-long
\defbeamertemplate{mini frame}{scaled circle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.13ex}
    \pgfpathcircle{\pgfpoint{.5ex}{.5ex}}{.5ex}
    \pgfusepath{fill,stroke}
  \end{pgfpicture}%
}
[action]
{
  \usebeamerfont{footer}%
  \setbeamersize{mini frame size=1.4ex,mini frame offset=.4ex}
}

\defbeamertemplate{mini frame in current section}{scaled circle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.13ex}
    \pgfpathcircle{\pgfpoint{.5ex}{.5ex}}{.5ex}
    \pgfusepath{stroke}
  \end{pgfpicture}%
}

\defbeamertemplate{mini frame in current subsection}{scaled circle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.13ex}
    \pgfpathcircle{\pgfpoint{.5ex}{.5ex}}{.5ex}
    \pgfusepath{stroke}
  \end{pgfpicture}%
}

\defbeamertemplate{mini frame}{scaled rectangle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.1ex}
    \pgfpathrectangle{\pgfpoint{0ex}{0ex}}{\pgfpoint{1ex}{1ex}}
    \pgfusepath{fill,stroke}
  \end{pgfpicture}%
}
[action]
{
  \usebeamerfont{footer}%
  \setbeamersize{mini frame size=1.4ex,mini frame offset=.4ex}
}

\defbeamertemplate{mini frame in current section}{scaled rectangle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.1ex}
    \pgfpathrectangle{\pgfpoint{0ex}{0ex}}{\pgfpoint{1ex}{1ex}}
    \pgfusepath{stroke}
  \end{pgfpicture}%
}

\defbeamertemplate{mini frame in current subsection}{scaled rectangle}
{%
  \usebeamerfont{footer}%
  \begin{pgfpicture}{0pt}{0pt}{1ex}{1ex}
    \pgfsetlinewidth{.1ex}
    \pgfpathrectangle{\pgfpoint{0ex}{0ex}}{\pgfpoint{1ex}{1ex}}
    \pgfusepath{stroke}
  \end{pgfpicture}%
}

\setbeamertemplate{mini frame}[scaled circle]
\setbeamertemplate{mini frame in current section}[scaled circle]
\setbeamertemplate{mini frame in current subsection}[scaled circle]

%%
%% /Section-Punkte, die mit der Schriftgröße skalieren
%%

\setbeamerfont{page number in head/foot}{series=\bfseries}

\newlength{\kitbottomdepth}
\newlength{\kitbottomheight}
\newlength{\kitfootergroupwidth}

% Standardfußzeile Vollbreite
\defbeamertemplate{footline}{KIT 2025}{%
  %% die "%" am Ende sind nötig, damit keine Abstände eingefügt werden
  % Falls kein Gruppenname angegeben, die ganze Breite der Fußzeile für den Titel nutzen.
  \ifdefined\@groupname%
    \ifx\@groupname\empty%
      \setlength{\kitfootergroupwidth}{0mm}%
      \else%
      % Falls die Breite des Gruppennamens definiert ist, diese nehmen, sonst 89 mm
      \ifdefined\@groupnamewidth%
        \setlength{\kitfootergroupwidth}{\@groupnamewidth}%
        \else%
        \setlength{\kitfootergroupwidth}{89mm}%
      \fi%
    \fi%
  \else%
      \setlength{\kitfootergroupwidth}{0mm}%
  \fi%
  \usebeamerfont{footer}%
  % Baseline so setzen, dass Text ungefähr mittig
  \setlength{\kitbottomdepth}{\dimexpr.5\kitbottom-.3\baselineskip\relax}%
  \setlength{\kitbottomheight}{\dimexpr.5\kitbottom+.7\baselineskip\relax}%
  %
  % Ränder rechts und links optisch durch Ausprobieren gesetzt
  \ifsectionnavigation%
    \begin{beamercolorbox}[wd=\paperwidth, leftskip=7.5mm, rightskip=7.5mm]{}%
      \insertnavigation{\dimexpr\paperwidth-15mm\relax}%
    \end{beamercolorbox}%
  \fi%
  \leavevmode%
  \kitfootlinepagenumber%
  \kitfootlinedate%
  % Die Boxen mit dem Titel und dem Gruppennamen sind vertikal zentriert, damit auch zweizeilige Texte schön aussehen
  % Daher müssen sie um \kitbottomdepth nach unten verschoben werden
  \raisebox{-\kitbottomdepth}{%
    % Die Box hat daher auch Höhe \kitbottom und Tiefe 0mm
    \kitfootlinetitle{\dimexpr251mm-\kitfootergroupwidth\relax}%
    \ifdefined\@groupname%
      \kitfootlinegroupname%
    \fi% 
  }%
  % KIT logo
  \ifdefined\@footlinelogo\else\footlinelogo{rgb}\fi
  \kitfootlinelogo{kitlogo_\@footlinelogo}%
}

% Fußzeile variabler Breite für Folien mit vollflächigem Bild-Anteil: ohne Section-Navigation, ohne Gruppennamen
\defbeamertemplate{footline}{KIT 2025 picture}[1]{%
  \usebeamerfont{footer}%
  % Baseline so setzen, dass Text ungefähr mittig
  \setlength{\kitbottomdepth}{\dimexpr.5\kitbottom-.3\baselineskip\relax}%
  \setlength{\kitbottomheight}{\dimexpr.5\kitbottom+.7\baselineskip\relax}%
  \leavevmode%
  \kitfootlinepagenumber%
  \kitfootlinedate%
  % Die Box mit dem Titel ist sind vertikal zentriert, damit auch zweizeilige Texte schön aussehen
  % Daher müssen sie um \kitbottomdepth nach unten verschoben werden
  \raisebox{-\kitbottomdepth}{%
    % Die Box hat daher auch Höhe \kitbottom und Tiefe 0mm
    \kitfootlinetitle{#1}%
  }%
  % Kein Gruppenname
  \hskip\dimexpr251mm-#1\relax%
  % KIT logo
  \ifdefined\@footlinelogo\else\footlinelogo{rgb}\fi
  \kitfootlinelogo{kitlogo_\@footlinelogo}%
}

% Seitennummer
\newcommand{\kitfootlinepagenumber}{%
  \begin{beamercolorbox}[wd=\dimexpr\kitoutermargin+16mm\relax, ht=\kitbottomheight, dp=\kitbottomdepth, leftskip=\kitoutermargin]{footer}
      \usebeamerfont{page number in head/foot}%
      \strut\insertframenumber{}/\inserttotalframenumber%
  \end{beamercolorbox}%
}

% Datum
\newcommand{\kitfootlinedate}{%
  \begin{beamercolorbox}[wd=31mm, ht=\kitbottomheight, dp=\kitbottomdepth]{footer}%
      \usebeamerfont{date in head/foot}%
      \strut\insertshortdate%
  \end{beamercolorbox}%
}

% Autor/Titel in Breite #1
\newcommand{\kitfootlinetitle}[1]{%
    \begin{beamercolorbox}[wd=#1, ht=\kitbottom, dp=0mm]{footer}%
        % Inhalt vertikal zentrieren; Anpassung um 1.5 pt, damit bei einzeiligem Inhalt genau die Baseline der Blöcke mit Seitenzahl und Datum getroffen wird
        \vbox to\kitbottom{\vfill\vskip\kitfooterbaselinecorrection%
          \strut\beamer@shortauthor\ifx\beamer@shortauthor\empty\else: \fi\beamer@shorttitle%
        \vfill}%
    \end{beamercolorbox}%
}

% Gruppenname
\newcommand{\kitfootlinegroupname}{%
    \begin{beamercolorbox}[wd=\kitfootergroupwidth, ht=\kitbottom, dp=0mm]{footer}
        \vbox to\kitbottom{\vfill\vskip\kitfooterbaselinecorrection%
          \strut\@groupname%
        \vfill}%
    \end{beamercolorbox}%
}

% KIT-Logo, Dateiname #1
\newcommand{\kitfootlinelogo}[1]{%
    \begin{beamercolorbox}[wd=18mm, ht=\kitbottomheight, dp=\kitbottomdepth]{footer}
      \raisebox{-4pt}{
        \includegraphics[width=\linewidth]{logos/#1}
      }
    \end{beamercolorbox}%
}

% ┌───────────┐
% │ /Fußzeile │
% └───────────┘

% ┌─────────────────┐
% │ Spezial-Layouts │
% └─────────────────┘

% Text/Bild vertikal geteilt
\define@key{beamerframe}{picture 33 vertical}[true]{%
  \kitpicturevertical{\kitcolumnfiveleft}
}

\define@key{beamerframe}{picture 50 vertical}[true]{%
  \kitpicturevertical{\kitcolumnfourleft}
}

\define@key{beamerframe}{picture 66 vertical}[true]{%
  \kitpicturevertical{\kitcolumnthreeleft}
}

\define@key{beamerframe}{picture vertical}[51]{%
  \kitpicturevertical{\dimexpr\paperwidth*(100-#1)/100\relax}
}

\newcommand{\kitpicturevertical}[1]{
  \setbeamertemplate{frametitle}[KIT 2025]{\dimexpr#1-\kitoutermargin-\kitcolumnsep\relax}
  \setbeamertemplate{footline}[KIT 2025 picture]{\dimexpr#1-\kitoutermargin-\kitcolumn\relax}
  \newgeometry{top=0.5cm,bottom=0cm,left=\kitoutermargin,right=\dimexpr\paperwidth-#1+\kitcolumnsep\relax}
  \setbeamertemplate{background}{\hskip#1\includegraphics[height=\paperheight]{\@titleimage}}
}
 
% Inhaltsverzeichnis

\define@key{beamerframe}{tableofcontents}[green]{%
  \setbeamertemplate{background}{
    \begin{beamercolorbox}[ht=\paperheight,wd=\kitcolumnsixleft]{#1 block}
    \end{beamercolorbox}
  }
  \setbeamercolor{section in toc}{fg=white}
  \setbeamercolor{subsection in toc}{fg=white}
  \setbeamertemplate{logo}[KIT 2025 title/toc]{kitlogo_rgb}
  \setbeamertemplate{frametitle}[KIT 2025 title in toc]
  \setbeamertemplate{footline}{}
}

\defbeamertemplate{frametitle}{KIT 2025 title in toc}{
    \begin{textblock*}{\kitcolumn}(\kitoutermargin,\kitoutermargin)
      \begin{beamercolorbox}[wd=\kitcolumn]{}
        \usebeamerfont*{frametitle}{\large\color{white}\insertframetitle}
      \end{beamercolorbox}
    \end{textblock*}
}

% siehe README.md
\define@key{beamerframe}{picture}{%
  \titleimage{#1}
}

\define@key{beamerframe}{kitlogo}[rgb]{%
  \titlelogo{#1}
  \footlinelogo{#1}
}

% ┌──────────────────┐
% │ /Spezial-Layouts │
% └──────────────────┘

%% ┏━━━━━━━━━━━━━━━━┓
%% ┃ /Folien-Layout ┃
%% ┗━━━━━━━━━━━━━━━━┛

%% ┏━━━━━━━━━━━━┓
%% ┃ Titelseite ┃
%% ┗━━━━━━━━━━━━┛

\def\titleimage#1{\def\@titleimage{#1}}
\def\titlelogo#1{\def\@titlelogo{#1}}
\def\grouplogo#1{\def\@grouplogo{#1}}

%% Frame-Optionen

% vertikale Layouts
\define@key{beamerframe}{title white vertical}[true]{%
  \setbeamercolor*{title}{parent=title on green}
  \setbeamercolor*{subtitle}{parent=subtitle on green}
  \setbeamercolor*{title text box}{parent=green block}
  \setbeamercolor*{title logo box}{parent=white block}
  \setbeamertemplate{background}[KIT 2025 title vertical]
  \setbeamertemplate{logo}[KIT 2025 title/toc]{kitlogo_rgb}
  \setbeamertemplate{title page}[KIT 2025 vertical]
  \setbeamertemplate{frametitle}{}
  \setbeamertemplate{footline}{}
}

\define@key{beamerframe}{title blue vertical}[true]{%
  \setbeamercolor*{title}{parent=title on green}
  \setbeamercolor*{subtitle}{parent=subtitle on green}
  \setbeamercolor*{title text box}{parent=green block}
  \setbeamercolor*{title logo box}{parent=blue block}
  \setbeamertemplate{background}[KIT 2025 title vertical]
  \setbeamertemplate{logo}[KIT 2025 title/toc]{kitlogo_white}
  \setbeamertemplate{title page}[KIT 2025 vertical]
  \setbeamertemplate{frametitle}{}
  \setbeamertemplate{footline}{}
}

% horizontale Layouts
\define@key{beamerframe}{title green horizontal}[true]{%
  \setbeamercolor*{title}{parent=title on green}
  \setbeamercolor*{subtitle}{parent=subtitle on green}
  \setbeamercolor*{title logo box}{parent=green block}
  \setbeamercolor*{title text box}{parent=green block}
  \setbeamertemplate{background}[KIT 2025 title horizontal]
  \setbeamertemplate{frametitle}[KIT 2025 title horizontal]
  \setbeamertemplate{footline}[KIT 2025 title horizontal]
  % KIT-Logo nach Definition, sonst bunt
  \ifdefined\@titlelogo\else\titlelogo{rgb}\fi
  \setbeamertemplate{logo}[KIT 2025 title/toc]{kitlogo_\@titlelogo}
  \setbeamertemplate{title page}[KIT 2025 horizontal]
}

\define@key{beamerframe}{title white horizontal}[true]{%
  \setbeamercolor*{title}{parent=title on white}
  \setbeamercolor*{subtitle}{parent=subtitle on white}
  \setbeamercolor*{title logo box}{parent=white block}
  \setbeamercolor*{title text box}{parent=white block}
  \setbeamertemplate{background}[KIT 2025 title horizontal]
  \setbeamertemplate{frametitle}[KIT 2025 title horizontal]
  \setbeamertemplate{footline}[KIT 2025 title horizontal]
  % KIT-Logo nach Definition, sonst bunt
  \ifdefined\@titlelogo\else\titlelogo{rgb}\fi
  \setbeamertemplate{logo}[KIT 2025 title/toc]{kitlogo_\@titlelogo}
  \setbeamertemplate{title page}[KIT 2025 horizontal]
}

%%
%% Absolut positionierte Elemente
%%
%
% From textpos documentation (https://ctan.org/pkg/textpos)
%
% \begin{textblock}{<hsize>}[<ho>,<vo>](<hpos>,<vpos>)
%   The coordinates <ho> and <vo> are fractions of the width and height of the text
%   box, respectively, and state that the box is to be placed so that the reference point
%   (<ho>,<vo>) within the box is to be placed at the point (<hpos>,<vpos>) on the page.

% KIT-Logo
\newcommand{\kittitlekitlogo}[1]{
  \begin{textblock*}{24mm}(297.5mm,\kitoutermargin)
  \includegraphics[width=24mm]{logos/#1} 
  \end{textblock*}
}

% Titel auf vertikaler Höhe #1 mit Breite #2
\newcommand{\kittitletitle}[2]{
  \begin{textblock*}{#2}(\kitoutermargin,#1)
    \begin{beamercolorbox}[wd=#2]{title}
      \usebeamerfont*{title}\inserttitle
    \end{beamercolorbox}
  \end{textblock*}
}

% Untertitel/Autor/Datum an Unterkante mit Breite #1
\newcommand{\kittitlesubtitle}[1]{
  \begin{textblock*}{#1}[0,1](\kitoutermargin,\dimexpr\paperheight-\kitbottom\relax)
    \begin{beamercolorbox}[wd=#1]{subtitle}
      \usebeamerfont*{subtitle}\insertsubtitle \ifx\@author\@empty\else\\[.2em]\fi %
      \usebeamerfont*{author}\insertauthor \ifx\@date\@empty\else~\textbar~\fi %
      \usebeamerfont*{date}\insertdate %
    \end{beamercolorbox}
  \end{textblock*}
}

%%
%% Gruppenlogo
%%

\newcommand{\kittitlegrouplogo}{
  \ifdefined\@grouplogo
    \ifx\@grouplogo\empty \else
      \includegraphics[width=40mm,height=40mm,keepaspectratio]{logos/\@grouplogo} 
    \fi % falls \grouplogo{} aufgerufen wird, kein Gruppenlogo einbinden
    \else % falls \grouplogo gar nicht aufgerufen wurde, Text anzeigen
      \colorbox{kit-purple100!20}{\parbox[b][18mm][c]{35mm}{\color{kit-purple100}\scriptsize\centering 
        \ifgerman
        Bitte Logo über \texttt{\textbackslash grouplogo\{\}} festlegen.
        \else
        Please set a logo using \texttt{\textbackslash grouplogo\{\}}.
        \fi
        }}
  \fi 
}

%%
%% Template-Definitionen
%%

\defbeamertemplate{title page}{KIT 2025 vertical}{
  \kittitletitle{\kitoutermargin}{\dimexpr\kitthreecolumns-\kitcolumnsep\relax}
  \kittitlesubtitle{\dimexpr\kitthreecolumns-\kitcolumnsep\relax}
}

% vertikales Layout

\defbeamertemplate{background}{KIT 2025 title vertical}{
    \begin{beamercolorbox}[ht=\paperheight,wd=\kitcolumnthreeright]{title text box}
    \end{beamercolorbox}%
    \begin{beamercolorbox}[ht=\paperheight,wd=\dimexpr2\kitcolumn+3\kitcolumnsep\relax]{title logo box}%
    \ifdefined\@titleimage%
      \ifx\@titleimage\empty\else%
      \includegraphics[height=\paperheight]{\@titleimage}%
      \fi%
    \fi%
    \end{beamercolorbox}%
    \begin{beamercolorbox}[ht=\paperheight,wd=\dimexpr\kitcolumn+\kitoutermargin\relax,right,rightskip=\kitoutermargin]{title logo box}
      \kittitlegrouplogo
      \vskip\kitbottom
    \end{beamercolorbox}
}

% horizontales Layout

\defbeamertemplate{title page}{KIT 2025 horizontal}{
  \kittitletitle{\kitrowfourtop}{\kitfivecolumns}
  \kittitlesubtitle{\kitfivecolumns}
  \frametitle{none} % Damit die obere Hälfte gerendert wird
}

% Im horizontalen Modus ist der Frametitle die obere Hälfte des Titels
\defbeamertemplate{frametitle}{KIT 2025 title horizontal}{
    \begin{beamercolorbox}[ht=\kitrowthreebottom,wd=\paperwidth]{title logo box}%
    \ifdefined\@titleimage%
      \ifx\@titleimage\empty\else%
      \includegraphics[width=\paperwidth]{\@titleimage}%
      \fi%
    \fi%
    \end{beamercolorbox}%
}

% Im horizontalen Modus ist die Footline die untere Hälfte des Titels
\defbeamertemplate{footline}{KIT 2025 title horizontal}{
    \begin{beamercolorbox}[ht=\dimexpr2\kitrowsep+2\kitrow+\kitbottom\relax,wd=\paperwidth,right,rightskip=\kitoutermargin]{title logo box}%
      \kittitlegrouplogo%
      \vskip\kitbottom
    \end{beamercolorbox}%
}

%% ┏━━━━━━━━━━━━━┓
%% ┃ /Titelseite ┃
%% ┗━━━━━━━━━━━━━┛

%% ┏━━━━━━━━┓
%% ┃ Farben ┃
%% ┗━━━━━━━━┛
%% KIT-Farbschema

% KIT color  green :
\definecolor{kit-green}{RGB}{0, 150, 130}
\colorlet{kit-green100}{kit-green}
\colorlet{kit-green90}{kit-green!90!white}
\colorlet{kit-green80}{kit-green!80!white}
\colorlet{kit-green70}{kit-green!70!white}
\colorlet{kit-green60}{kit-green!60!white}
\colorlet{kit-green50}{kit-green!50!white}
\colorlet{kit-green40}{kit-green!40!white}
\colorlet{kit-green30}{kit-green!30!white}
\colorlet{kit-green25}{kit-green!25!white}
\colorlet{kit-green20}{kit-green!20!white}
\colorlet{kit-green15}{kit-green!15!white}
\colorlet{kit-green10}{kit-green!10!white}
\colorlet{kit-green5}{kit-green!5!white}

% KIT color blue:
\definecolor{kit-blue}{RGB}{70, 100, 170}
\colorlet{kit-blue100}{kit-blue}
\colorlet{kit-blue90}{kit-blue!90!white}
\colorlet{kit-blue80}{kit-blue!80!white}
\colorlet{kit-blue70}{kit-blue!70!white}
\colorlet{kit-blue60}{kit-blue!60!white}
\colorlet{kit-blue50}{kit-blue!50!white}
\colorlet{kit-blue40}{kit-blue!40!white}
\colorlet{kit-blue30}{kit-blue!30!white}
\colorlet{kit-blue25}{kit-blue!25!white}
\colorlet{kit-blue20}{kit-blue!20!white}
\colorlet{kit-blue15}{kit-blue!15!white}
\colorlet{kit-blue10}{kit-blue!10!white}
\colorlet{kit-blue5}{kit-blue!5!white}

\definecolor{kit-royalblue}{RGB}{0, 45, 76}
\colorlet{kit-royalblue100}{kit-royalblue}
\colorlet{kit-royalblue90}{kit-royalblue!90!white}
\colorlet{kit-royalblue80}{kit-royalblue!80!white}
\colorlet{kit-royalblue70}{kit-royalblue!70!white}
\colorlet{kit-royalblue60}{kit-royalblue!60!white}
\colorlet{kit-royalblue50}{kit-royalblue!50!white}
\colorlet{kit-royalblue40}{kit-royalblue!40!white}
\colorlet{kit-royalblue30}{kit-royalblue!30!white}
\colorlet{kit-royalblue25}{kit-royalblue!25!white}
\colorlet{kit-royalblue20}{kit-royalblue!20!white}
\colorlet{kit-royalblue15}{kit-royalblue!15!white}
\colorlet{kit-royalblue10}{kit-royalblue!10!white}
\colorlet{kit-royalblue5}{kit-royalblue!5!white}

% Powerpoint-"Eisblau"
\definecolor{kit-iceblue100}{RGB}{30, 53, 69}
\definecolor{kit-iceblue70}{RGB}{68, 94, 111}
\definecolor{kit-iceblue50}{RGB}{168, 185, 196}
\definecolor{kit-iceblue30}{RGB}{218, 225, 230}

% KIT color  red :
\definecolor{kit-red}{RGB}{162, 34, 35}
\colorlet{kit-red100}{kit-red}
\colorlet{kit-red90}{kit-red!90!white}
\colorlet{kit-red80}{kit-red!80!white}
\colorlet{kit-red70}{kit-red!70!white}
\colorlet{kit-red60}{kit-red!60!white}
\colorlet{kit-red50}{kit-red!50!white}
\colorlet{kit-red40}{kit-red!40!white}
\colorlet{kit-red30}{kit-red!30!white}
\colorlet{kit-red25}{kit-red!25!white}
\colorlet{kit-red20}{kit-red!20!white}
\colorlet{kit-red15}{kit-red!15!white}
\colorlet{kit-red10}{kit-red!10!white}
\colorlet{kit-red5}{kit-red!5!white}

% KIT color  yellow :
\definecolor{kit-yellow}{RGB}{252, 229, 0}
\colorlet{kit-yellow100}{kit-yellow}
\colorlet{kit-yellow90}{kit-yellow!90!white}
\colorlet{kit-yellow80}{kit-yellow!80!white}
\colorlet{kit-yellow70}{kit-yellow!70!white}
\colorlet{kit-yellow60}{kit-yellow!60!white}
\colorlet{kit-yellow50}{kit-yellow!50!white}
\colorlet{kit-yellow40}{kit-yellow!40!white}
\colorlet{kit-yellow30}{kit-yellow!30!white}
\colorlet{kit-yellow25}{kit-yellow!25!white}
\colorlet{kit-yellow20}{kit-yellow!20!white}
\colorlet{kit-yellow15}{kit-yellow!15!white}
\colorlet{kit-yellow10}{kit-yellow!10!white}
\colorlet{kit-yellow5}{kit-yellow!5!white}

% KIT color  orange :
\definecolor{kit-orange}{RGB}{223, 155, 27}
\colorlet{kit-orange100}{kit-orange}
\colorlet{kit-orange90}{kit-orange!90!white}
\colorlet{kit-orange80}{kit-orange!80!white}
\colorlet{kit-orange70}{kit-orange!70!white}
\colorlet{kit-orange60}{kit-orange!60!white}
\colorlet{kit-orange50}{kit-orange!50!white}
\colorlet{kit-orange40}{kit-orange!40!white}
\colorlet{kit-orange30}{kit-orange!30!white}
\colorlet{kit-orange25}{kit-orange!25!white}
\colorlet{kit-orange20}{kit-orange!20!white}
\colorlet{kit-orange15}{kit-orange!15!white}
\colorlet{kit-orange10}{kit-orange!10!white}
\colorlet{kit-orange5}{kit-orange!5!white}

% KIT color  lightgreen :
\definecolor{kit-lightgreen}{RGB}{140, 182, 60}
\colorlet{kit-lightgreen100}{kit-lightgreen}
\colorlet{kit-lightgreen90}{kit-lightgreen!90!white}
\colorlet{kit-lightgreen80}{kit-lightgreen!80!white}
\colorlet{kit-lightgreen70}{kit-lightgreen!70!white}
\colorlet{kit-lightgreen60}{kit-lightgreen!60!white}
\colorlet{kit-lightgreen50}{kit-lightgreen!50!white}
\colorlet{kit-lightgreen40}{kit-lightgreen!40!white}
\colorlet{kit-lightgreen30}{kit-lightgreen!30!white}
\colorlet{kit-lightgreen25}{kit-lightgreen!25!white}
\colorlet{kit-lightgreen20}{kit-lightgreen!20!white}
\colorlet{kit-lightgreen15}{kit-lightgreen!15!white}
\colorlet{kit-lightgreen10}{kit-lightgreen!10!white}
\colorlet{kit-lightgreen5}{kit-lightgreen!5!white}


% KIT color  purple :
\definecolor{kit-purple}{RGB}{163, 16, 124}
\colorlet{kit-purple100}{kit-purple}
\colorlet{kit-purple90}{kit-purple!90!white}
\colorlet{kit-purple80}{kit-purple!80!white}
\colorlet{kit-purple70}{kit-purple!70!white}
\colorlet{kit-purple60}{kit-purple!60!white}
\colorlet{kit-purple50}{kit-purple!50!white}
\colorlet{kit-purple40}{kit-purple!40!white}
\colorlet{kit-purple30}{kit-purple!30!white}
\colorlet{kit-purple25}{kit-purple!25!white}
\colorlet{kit-purple20}{kit-purple!20!white}
\colorlet{kit-purple15}{kit-purple!15!white}
\colorlet{kit-purple10}{kit-purple!10!white}
\colorlet{kit-purple5}{kit-purple!5!white}

% KIT color  brown :
\definecolor{kit-brown}{RGB}{167, 130, 46}
\colorlet{kit-brown100}{kit-brown}
\colorlet{kit-brown90}{kit-brown!90!white}
\colorlet{kit-brown80}{kit-brown!80!white}
\colorlet{kit-brown70}{kit-brown!70!white}
\colorlet{kit-brown60}{kit-brown!60!white}
\colorlet{kit-brown50}{kit-brown!50!white}
\colorlet{kit-brown40}{kit-brown!40!white}
\colorlet{kit-brown30}{kit-brown!30!white}
\colorlet{kit-brown25}{kit-brown!25!white}
\colorlet{kit-brown20}{kit-brown!20!white}
\colorlet{kit-brown15}{kit-brown!15!white}
\colorlet{kit-brown10}{kit-brown!10!white}
\colorlet{kit-brown5}{kit-brown!5!white}

% KIT color  cyan :
\definecolor{kit-cyan}{RGB}{35, 161, 224}
\colorlet{kit-cyan100}{kit-cyan}
\colorlet{kit-cyan90}{kit-cyan!90!white}
\colorlet{kit-cyan80}{kit-cyan!80!white}
\colorlet{kit-cyan70}{kit-cyan!70!white}
\colorlet{kit-cyan60}{kit-cyan!60!white}
\colorlet{kit-cyan50}{kit-cyan!50!white}
\colorlet{kit-cyan40}{kit-cyan!40!white}
\colorlet{kit-cyan30}{kit-cyan!30!white}
\colorlet{kit-cyan25}{kit-cyan!25!white}
\colorlet{kit-cyan20}{kit-cyan!20!white}
\colorlet{kit-cyan15}{kit-cyan!15!white}
\colorlet{kit-cyan10}{kit-cyan!10!white}
\colorlet{kit-cyan5}{kit-cyan!5!white}

% KIT color  gray :
\definecolor{kit-gray}{RGB}{0, 0, 0}
\colorlet{kit-gray100}{kit-gray}
\colorlet{kit-gray90}{kit-gray!90!white}
\colorlet{kit-gray80}{kit-gray!80!white}
\colorlet{kit-gray70}{kit-gray!70!white}
\colorlet{kit-gray60}{kit-gray!60!white}
\colorlet{kit-gray50}{kit-gray!50!white}
\colorlet{kit-gray40}{kit-gray!40!white}
\colorlet{kit-gray30}{kit-gray!30!white}
\colorlet{kit-gray25}{kit-gray!25!white}
\colorlet{kit-gray20}{kit-gray!20!white}
\colorlet{kit-gray15}{kit-gray!15!white}
\colorlet{kit-gray10}{kit-gray!10!white}
\colorlet{kit-gray5}{kit-gray!5!white}

\setbeamercolor*{normal text}{fg=kit-royalblue}
\setbeamercolor*{alerted text}{fg=kit-red100}
\setbeamercolor*{example text}{fg=kit-royalblue}
\setbeamercolor*{structure}{fg=kit-royalblue}
\setbeamercolor*{frametitle}{fg=normal text.fg}
\setbeamercolor*{framesubtitle}{fg=kit-iceblue50}
\setbeamercolor*{footer}{fg=kit-iceblue50.fg}

\setbeamercolor*{palette primary}{fg=normal text.fg,bg=black!15}
\setbeamercolor*{palette secondary}{fg=normal text.fg,bg=black!15}
\setbeamercolor*{palette tertiary}{fg=normal text.fg,bg=black!15}
\setbeamercolor*{palette quaternary}{fg=normal text.fg,bg=black!15}

\setbeamercolor*{item projected}{fg=white,bg=kit-green100}
\setbeamercolor*{itemize item}{fg=kit-green100}
\setbeamercolor*{itemize subitem}{fg=kit-green100}
\setbeamercolor*{itemize subsubitem}{fg=kit-green100}

\setbeamercolor*{block title}{fg=white,bg=kit-green100}
\setbeamercolor*{block title alerted}{use=alerted text,fg=white,bg=alerted text.fg!75!black}
\setbeamercolor*{block title example}{fg=white,bg=kit-royalblue}

\setbeamercolor*{green block}{bg=kit-green100}
\setbeamercolor*{white block}{bg=white}
\setbeamercolor*{blue block}{bg=kit-royalblue100}

\setbeamercolor*{title on green}{fg=kit-green20}
\setbeamercolor*{title on white}{fg=kit-green100}

\setbeamercolor*{subtitle on green}{fg=white}
\setbeamercolor*{subtitle on white}{fg=normal text.fg}

\setbeamercolor*{block body}{fg=normal text.fg,bg=kit-green15}
\setbeamercolor*{block body alerted}{parent=normal text,use=block title alerted,bg=block title alerted.bg!10!bg}
\setbeamercolor*{block body example}{fg=normal text.fg,bg=kit-blue15}

\setbeamercolor*{standard box}{fg=white,bg=kit-green100}%
\setbeamercolor*{highlight box}{fg=white,bg=kit-royalblue100}%
\setbeamercolor*{highlight box gray}{fg=normal text.fg,bg=kit-iceblue50}%
\setbeamercolor*{highlight box lightgray}{fg=normal text.fg,bg=kit-iceblue30}%

\setbeamercolor*{standard box framed}{fg=kit-green100,bg=}%
\setbeamercolor*{highlight box framed}{fg=kit-royalblue100,bg=}%
\setbeamercolor*{highlight box gray framed}{fg=kit-iceblue50,bg=}%

\setbeamercolor*{background canvas}{bg=white}

\setbeamercolor*{footer}{fg=kit-royalblue100!50!white}

\setbeamercolor*{mini frame}{fg=kit-royalblue100!50!white}
\setbeamercolor{section in head/foot}{fg=kit-royalblue100!50!white}

%% ┏━━━━━━━━━┓
%% ┃ /Farben ┃
%% ┗━━━━━━━━━┛

\setbeamertemplate{section in toc}{{\inserttocsectionnumber.~\inserttocsection}\\[.2em]}
\setbeamercolor{section in toc}{fg=kit-royalblue}
\setbeamerfont{section in toc}{size=\huge, series=\bfseries}
\setbeamerfont{subsection in toc}{size=\large}

%% ┏━━━━━━━━━━━━━━━━━━━━┓
%% ┃ Block-Definitionen ┃
%% ┗━━━━━━━━━━━━━━━━━━━━┛

% Original von tex/latex/beamer/beamerinnerthemedefault.sty
% colsep* durch colsep ersetzt, damit die Blöcke nicht über das Raster herausragen
\defbeamertemplate*{block begin}{KIT 2025}
{
  \par\vskip\medskipamount%
  \begin{beamercolorbox}[colsep=.75ex]{block title}
    \usebeamerfont*{block title}\insertblocktitle%
  \end{beamercolorbox}%
  {\parskip0pt\par}%
  \ifbeamercolorempty[bg]{block title}
  {}
  {\ifbeamercolorempty[bg]{block body}{}{\nointerlineskip\vskip-0.5pt}}%
  \usebeamerfont{block body}%
  \begin{beamercolorbox}[colsep=.75ex,vmode]{block body}%
    \ifbeamercolorempty[bg]{block body}{\vskip-.25ex}{\vskip-.75ex}\vbox{}%
}
\defbeamertemplate*{block end}{KIT 2025}  
{\end{beamercolorbox}\vskip\smallskipamount}

\defbeamertemplate*{block alerted begin}{KIT 2025}
{
  \par\vskip\medskipamount%
  \begin{beamercolorbox}[colsep=.75ex]{block title alerted}
    \usebeamerfont*{block title alerted}\insertblocktitle%
  \end{beamercolorbox}%
  {\parskip0pt\par}%
  \ifbeamercolorempty[bg]{block title alerted}
  {}
  {\ifbeamercolorempty[bg]{block body alerted}{}{\nointerlineskip\vskip-0.5pt}}%
  \usebeamerfont{block body alerted}%
  \begin{beamercolorbox}[colsep=.75ex,vmode]{block body alerted}%
    \ifbeamercolorempty[bg]{block body alerted}{\vskip-.25ex}{\vskip-.75ex}\vbox{}%
}

\defbeamertemplate*{block alerted end}{KIT 2025}
{\end{beamercolorbox}\vskip\smallskipamount}

\defbeamertemplate*{block example begin}{KIT 2025}
{
  \par\vskip\medskipamount%
  \begin{beamercolorbox}[colsep=.75ex]{block title example}
    \usebeamerfont*{block title example}\insertblocktitle%
  \end{beamercolorbox}%
  {\parskip0pt\par}%
  \ifbeamercolorempty[bg]{block title example}
  {}
  {\ifbeamercolorempty[bg]{block body example}{}{\nointerlineskip\vskip-0.5pt}}%
  \usebeamerfont{block body example}%
  \begin{beamercolorbox}[colsep=.75ex,vmode]{block body example}%
    \ifbeamercolorempty[bg]{block body example}{\vskip-.25ex}{\vskip-.75ex}\vbox{}%
}
\defbeamertemplate*{block example end}{KIT 2025}
{\end{beamercolorbox}\vskip\smallskipamount}

\setbeamertemplate{block begin}[KIT 2025]
\setbeamertemplate{block end}[KIT 2025]
\setbeamertemplate{block alerted begin}[KIT 2025]
\setbeamertemplate{block alerted end}[KIT 2025]
\setbeamertemplate{block example begin}[KIT 2025]
\setbeamertemplate{block example end}[KIT 2025]

% Content environment for structuring. Basically a headline followed by text
\newenvironment<>{contentblock}[1]{\begingroup%
  \setbeamertemplate{blocks}[default]
	\setbeamercolor{block body}{fg=normal text.fg,bg=}%
  \setbeamercolor{block title}{fg=normal text.fg,bg=}%
  \setbeamerfont*{block title}{family=\sffamily,series=\bfseries,size=\large}
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{royalblueblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-iceblue30!50}%
	\setbeamercolor{block title}{fg=white,bg=kit-royalblue100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{grayblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-iceblue30!50}%
	\setbeamercolor{block title}{fg=normal text.fg,bg=kit-iceblue50}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{lightgrayblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-iceblue30!50}%
	\setbeamercolor{block title}{fg=normal text.fg,bg=kit-iceblue30}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{greenblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-green15}%
	\setbeamercolor{block title}{fg=white,bg=kit-green100}%
	\begin{block}#2{#1}%
	}{\end{block}
\endgroup}

\newenvironment<>{blueblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-blue15}%
	\setbeamercolor{block title}{fg=white,bg=kit-blue100}%
	\begin{block}#2{#1}%
	}{\end{block}
\endgroup}

\newenvironment<>{redblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-red15}%
	\setbeamercolor{block title}{fg=white,bg=kit-red100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{brownblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-brown15}%
	\setbeamercolor{block title}{fg=white,bg=kit-brown100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{purpleblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-purple15}%
	\setbeamercolor{block title}{fg=white,bg=kit-purple100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{yellowblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-yellow30}%
	\setbeamercolor{block title}{fg=normal text.fg,bg=kit-yellow100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{lightgreenblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-lightgreen15}%
	\setbeamercolor{block title}{fg=white,bg=kit-lightgreen100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{orangeblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-orange15}%
	\setbeamercolor{block title}{fg=white,bg=kit-orange100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

\newenvironment<>{cyanblock}[1]{\begingroup%
	\setbeamercolor{block body}{fg=normal text.fg,bg=kit-cyan15}%
	\setbeamercolor{block title}{fg=white,bg=kit-cyan100}%
	\begin{block}#2{#1}%
	}{\end{block}%
\endgroup}

%%
%% Boxen
%%

\newenvironment<>{standardbox}{%
  \begin{beamercolorbox}[sep=1em,center,vmode]{standard box}%
  \usebeamerfont{standard box}%
}{%
  \end{beamercolorbox}%
}

\newenvironment<>{highlightbox}{%
  \begin{beamercolorbox}[sep=1em,center,vmode]{highlight box}
  \usebeamerfont{highlight box}%
}{%
  \end{beamercolorbox}%
}

\newenvironment<>{grayhighlightbox}{%
  \begin{beamercolorbox}[sep=1em,center,vmode]{highlight box gray}
  \usebeamerfont{highlight box}%
}{%
  \end{beamercolorbox}%
}

\newenvironment<>{lightgrayhighlightbox}{%
  \begin{beamercolorbox}[sep=1em,center,vmode]{highlight box lightgray}
  \usebeamerfont{highlight box}%
}{%
  \end{beamercolorbox}%
}

%% ┏━━━━━━━━━━━━━━━━━━━━━┓
%% ┃ /Block-Definitionen ┃
%% ┗━━━━━━━━━━━━━━━━━━━━━┛

%% Appendix-Seiten zählen nicht in die Gesamtfolienzahl

\newcommand{\beginbackup}{
   \newcounter{framenumbervorappendix}
   \setcounter{framenumbervorappendix}{\value{framenumber}}
}
\newcommand{\backupend}{
   \addtocounter{framenumbervorappendix}{-\value{framenumber}}
   \addtocounter{framenumber}{\value{framenumbervorappendix}} 
}