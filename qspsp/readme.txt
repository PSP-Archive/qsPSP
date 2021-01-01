qsPSP v.0.95 (QSP 5.5.0)

qsPSP представляет собой порт платформы текстовых игр QSP на портативную игровую приставку Sony PSP.

Игры для qsPSP, а так же версию платформы под Windows вы можете скачать с сайта http://qsp.su.
Новые версии qspPSP вы сможете скачать с сайта http://elmortem.ru.

_________
УСТАНОВКА

Для установки на 3.71 (в том числе Slim) скопируйте только папку qsPSP/ в директорию с вашими программами и играми на PSP.
Для установки на 1.50 скопируйте папку qsPSP/ и папку qsPSP%/ в директорию с вашими программами и играми на PSP.

_____
ВАЖНО

Если в игре используются HTML-ссылки, то скорее всего поиграть в игру не удастся, т.к. HTML-рендеринга пока нет, работаю над этим.

__________
УПРАВЛЕНИЕ

	Up\Down - выбрать пункт из активного списка.
	Analog - скроллинг активной текстовой области.
	LTrigger - сменить активную закладку.
	RTrigger - при удерживании: сменить активную закладку.
	Circle - в меню: активация пункта, в пользовательском вводе: выбор символа.
	Cross - в меню и диалогах: отмена.
	Triangle - ввод пользовательского текста, изменение раскладки.
	Square - в пользовательском вводе: удаление символа.
	Start - сохранение и загрузка (Triangle - изменение режима)
	Select - выбор и загрузка квеста

_____
СКИНЫ

С этой версии программа начала поддерживать скины, которые можно включать в свой квест, для придания ему должного вида.
Скин состоит из 6 файлов:
название_font.fb - берётся оригинальный из папки data/ и переименовывается.
skin_00_font.png - берётся оригинальный из папки data/.
название_main.jpg - картинка основого окна.
название_msg.jpg - картинка сообщения.
название_menu.jpg - картинка меню.
название_input.jpg - картинка ввода текста.
Смотри пример Input/input.gam и директорию games/Input/

За основу можно взять графику из папки data/ или games/Input/

Чтобы загрузить скин из своего квеста в первой локации вы должны в переменной $SKINNAME указать название вашего скина.
Пример:
$SKINNAME = 'wb'

Так же из квеста можно поменять цвет рамки активной панели и цвет выделенного элемента меню.
Пример:
! меняем цвет рамки
RCOLOR = RGB(10, 10, 255)
! меняем цвет выделенного элемента меню
SCOLOR = RGB(255, 255, 10)


___________
МУЛЬТИМЕДИЯ

Подерживаются следующие форматы мультимедийных данных:
JPEG, PNG, TGA (картинка должна быть вверх ногами) - команда VIEW
MP3, WAV (PCM, 8 bit, Mono) - команда PLAY



(c) Osokin 'elmortem' Makar, 2007-2009========================================================================
    MAKEFILE PROJECT : qspsp Project Overview
========================================================================

AppWizard has created this qspsp project for you.  

This file contains a summary of what you will find in each of the files that
make up your qspsp project.


qspsp.vcproj
    This is the main project file for VC++ projects generated using an Application Wizard. 
    It contains information about the version of Visual C++ that generated the file, and 
    information about the platforms, configurations, and project features selected with the
    Application Wizard.

This project allows you to build/clean/rebuild from within Visual Studio by calling the commands you have input 
in the wizard. The build command can be nmake or any other tool you use.

This project does not contain any files, so there are none displayed in Solution Explorer.

/////////////////////////////////////////////////////////////////////////////
