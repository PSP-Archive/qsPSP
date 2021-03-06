/* Copyright (C) 2005-2009 Valeriy Argunov (nporep AT mail DOT ru) */
/*
* This library is free software; you can redistribute it and/or modify
* it under the terms of the GNU Lesser General Public License as published by
* the Free Software Foundation; either version 2.1 of the License, or
* (at your option) any later version.
*
* This library is distributed in the hope that it will be useful,
* but WITHOUT ANY WARRANTY; without even the implied warranty of
* MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
* GNU Lesser General Public License for more details.
*
* You should have received a copy of the GNU Lesser General Public License
* along with this library; if not, write to the Free Software
* Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
*/

#include "callbacks.h"
#include "actions.h"
#include "common.h"
#include "errors.h"
#include "objects.h"
#include "text.h"

QSP_CALLBACK qspCallBacks[QSP_CALL_DUMMY];
volatile QSP_BOOL qspIsInCallBack = QSP_FALSE;

static void qspSaveCallState(QSPCallState *);
static void qspRestoreCallState(QSPCallState *);

static void qspSaveCallState(QSPCallState *state)
{
	state->IsInCallBack = qspIsInCallBack;
	state->IsMustWait = qspIsMustWait;
	state->IsMainDescChanged = qspIsMainDescChanged;
	state->IsVarsDescChanged = qspIsVarsDescChanged;
	state->IsObjectsChanged = qspIsObjectsChanged;
	state->IsActionsChanged = qspIsActionsChanged;
	qspIsInCallBack = QSP_TRUE;
	qspIsMustWait = QSP_FALSE;
}

static void qspRestoreCallState(QSPCallState *state)
{
	qspResetError();
	if (state->IsActionsChanged) qspIsActionsChanged = QSP_TRUE;
	if (state->IsObjectsChanged) qspIsObjectsChanged = QSP_TRUE;
	if (state->IsVarsDescChanged) qspIsVarsDescChanged = QSP_TRUE;
	if (state->IsMainDescChanged) qspIsMainDescChanged = QSP_TRUE;
	qspIsMustWait = state->IsMustWait;
	qspIsInCallBack = state->IsInCallBack;
}

void qspInitCallBacks()
{
	long i;
	qspIsInCallBack = QSP_FALSE;
	for (i = 0; i < QSP_CALL_DUMMY; ++i)
		qspCallBacks[i] = 0;
}

void qspSetCallBack(long type, QSP_CALLBACK func)
{
	qspCallBacks[type] = func;
}

void qspCallDebug(QSP_CHAR *str)
{
	/* ����� �������� ���������� ��������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_DEBUG])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_DEBUG](str);
		qspRestoreCallState(&state);
	}
}

void qspCallSetTimer(long msecs)
{
	/* ����� ������������� �������� ������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SETTIMER])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SETTIMER](msecs);
		qspRestoreCallState(&state);
	}
}

void qspCallRefreshInt(QSP_BOOL isRedraw)
{
	/* ����� ��������� ���������� ���������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_REFRESHINT])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_REFRESHINT](isRedraw);
		qspRestoreCallState(&state);
	}
}

void qspCallSetInputStrText(QSP_CHAR *text)
{
	/* ����� ������������� ����� ������ ����� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SETINPUTSTRTEXT])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SETINPUTSTRTEXT](text);
		qspRestoreCallState(&state);
	}
}

void qspCallAddMenuItem(QSP_CHAR *name, QSP_CHAR *imgPath)
{
	/* ����� ��������� ����� ���� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_ADDMENUITEM])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_ADDMENUITEM](name, imgPath);
		qspRestoreCallState(&state);
	}
}

void qspCallSystem(QSP_CHAR *cmd)
{
	/* ����� ��������� ��������� ����� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SYSTEM])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SYSTEM](cmd);
		qspRestoreCallState(&state);
	}
}

void qspCallOpenGame()
{
	/* ����� ��������� ������������ ������� ���� */
	/* ��������� ���� ��� �������� � ��������� ��� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_OPENGAMESTATUS])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_OPENGAMESTATUS]();
		qspRestoreCallState(&state);
	}
}

void qspCallSaveGame()
{
	/* ����� ��������� ������������ ������� ���� */
	/* ��� ���������� ��������� ���� � ��������� */
	/* � ��� ������� ��������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SAVEGAMESTATUS])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SAVEGAMESTATUS]();
		qspRestoreCallState(&state);
	}
}

void qspCallShowMessage(QSP_CHAR *text)
{
	/* ����� ���������� ��������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SHOWMSGSTR])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SHOWMSGSTR](text);
		qspRestoreCallState(&state);
	}
}

void qspCallShowMenu()
{
	/* ����� ���������� ���� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SHOWMENU])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SHOWMENU]();
		qspRestoreCallState(&state);
	}
}

void qspCallShowPicture(QSP_CHAR *file)
{
	/* ����� ���������� ����������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SHOWIMAGE])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SHOWIMAGE](file);
		qspRestoreCallState(&state);
	}
}

void qspCallShowWindow(long type, QSP_BOOL isShow)
{
	/* ����� ���������� ��� �������� ���� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SHOWWINDOW])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SHOWWINDOW](type, isShow);
		qspRestoreCallState(&state);
	}
}

void qspCallPlayFile(QSP_CHAR *file, long volume)
{
	/* ����� �������� ��������������� ����� � �������� ���������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_PLAYFILE])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_PLAYFILE](file, volume);
		qspRestoreCallState(&state);
	}
}

QSP_BOOL qspCallIsPlayingFile(QSP_CHAR *file)
{
	/* ����� ���������, ������������� �� ���� */
	QSPCallState state;
	QSP_BOOL isPlaying;
	if (qspCallBacks[QSP_CALL_ISPLAYINGFILE])
	{
		qspSaveCallState(&state);
		isPlaying = (QSP_BOOL)qspCallBacks[QSP_CALL_ISPLAYINGFILE](file);
		qspRestoreCallState(&state);
		return isPlaying;
	}
	return QSP_FALSE;
}

void qspCallSleep(long msecs)
{
	/* ����� ������� �������� ���������� ����������� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_SLEEP])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_SLEEP](msecs);
		qspRestoreCallState(&state);
	}
}

long qspCallGetMSCount()
{
	/* ����� �������� ���������� �����������, ��������� � ������� ���������� ������ ������� */
	QSPCallState state;
	long count;
	if (qspCallBacks[QSP_CALL_GETMSCOUNT])
	{
		qspSaveCallState(&state);
		count = qspCallBacks[QSP_CALL_GETMSCOUNT]();
		qspRestoreCallState(&state);
		return count;
	}
	return 0;
}

void qspCallCloseFile(QSP_CHAR *file)
{
	/* ����� ��������� �������� ����� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_CLOSEFILE])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_CLOSEFILE](file);
		qspRestoreCallState(&state);
	}
}

void qspCallDeleteMenu()
{
	/* ����� ������� ������� ���� */
	QSPCallState state;
	if (qspCallBacks[QSP_CALL_DELETEMENU])
	{
		qspSaveCallState(&state);
		qspCallBacks[QSP_CALL_DELETEMENU]();
		qspRestoreCallState(&state);
	}
}

QSP_CHAR *qspCallInputBox(QSP_CHAR *text)
{
	/* ����� ������ ����� */
	QSPCallState state;
	QSP_CHAR *buffer;
	long maxLen = 511;
	if (qspCallBacks[QSP_CALL_INPUTBOX])
	{
		qspSaveCallState(&state);
		buffer = (QSP_CHAR *)malloc((maxLen + 1) * sizeof(QSP_CHAR));
		*buffer = 0;
		qspCallBacks[QSP_CALL_INPUTBOX](text, buffer, maxLen);
		buffer[maxLen] = 0;
		qspRestoreCallState(&state);
	}
	else
		buffer = qspGetNewText(QSP_FMT(""), 0);
	return buffer;
}
