

DWORD WINAPI ThreadProcSS(LPVOID lpvParam)
{
	HANDLE hCom;
	DWORD dwEvtMask = 0;
	OVERLAPPED o;
	BOOL fPending = 0;
	DWORD dwRet;
	HANDLE hArray[2];
	BOOL proc;

	memset(&o, 0, sizeof(o));

	o.hEvent = CreateEvent(NULL, TRUE, FALSE, NULL);

	if (o.hEvent == NULL)
	{
		printf("CreateEvent failed.\n");
		return 1;
	}

	hCom = CreateFile(L"\\\\.\\COM1",
		GENERIC_READ | GENERIC_WRITE,
		0, // exclusive access
		NULL, // no security attrs
		OPEN_EXISTING,
		FILE_FLAG_OVERLAPPED,
		NULL);

	if (hCom == INVALID_HANDLE_VALUE)
	{
		printf("unable to open COM port\n");
		CloseHandle(o.hEvent);
		return 1;
	}

	SetCommMask(hCom, EV_CTS | EV_DSR);

	for (;;)
	{
		proc = 0;

		if (!fPending)
		{
			if (WaitCommEvent(hCom, &dwEvtMask, &o))
				proc = 1;
			else {
				if (GetLastError() == ERROR_IO_PENDING)
					fPending = 1;
			}
		}
		else
		{
			dwRet = WaitForSingleObject(hCom, NULL);
			if (dwRet == WAIT_OBJECT_0) {
				proc = 1;
				fPending = 0;
			}
		}

		if (proc)
		{
			if (dwEvtMask & EV_DSR)
				printf("ev_dsr\n");

			if (dwEvtMask & EV_CTS)
				printf("ev_cts\n");
		}
	}

	CloseHandle(o.hEvent);

	CloseHandle(hCom);

	return 0;
}

https://microsoft.public.win32.programmer.kernel.narkive.com/tiNym0fO/waitcommevent-getlasterror-87-undefined

�� ������� SetCommMask(), ������� � �����������, ��� ����� ���������������
��������� ����� ������ ������� WaitCommEvent(), ������� ��������� �����,
��� ������ ��� ������������ ��� ���������� ������ WaitCommMask().
���������� ��������� SetCommMask() � ���� ����� WaitCommEvent().

�������� � ���, ��� WaitCommEvent ���������� �
�� �� ��������� ����������, ������ ��� �������
�������� ���������� ���������� ���� / �����.

� ������ ERROR_IO_PENDING ��� ������ ������������
a WaitForSingleObject, WaitForMultipleObjects ���
GetOverlappedResult, ����� ������ OVERLAPPED
���� hEvent �������� ������ ����� �������
WaitCommEvent �����.

� �������� ������, ������� ������ ������ ��, ��� �� ���������
���������.� ������������� ��� ��� ���������� ���������� Win32.

DwEvtMask ��� ������������� ���������������� ����� - ������ �� ������
����������� �� ���������� ����� / ������.��� �������� ����� ���������
�� ����������.����� GetCommMask �
��� ������� �� �����.

� ������� ���� ������������ ���� ����� ��� �������� COM1 �
������ ��� �������� ����� � ����������.������� ������� �����
������ ����������������� ������ ����������.


���� ��������������� �������� �� ����� ���� ��������� ����������, �������
���������� FALSE, � ������� GetLastError ���������� ERROR_IO_PENDING, ��������,
��� �������� ����������� � ������� ������.����� ��� ����������, �������
������������� ������� hEvent ��������� OVERLAPPED � ��������� ��� ������������
�� �������� WaitCommEvent, � ����� ������������� ��� � ��������� ������������,
����� ���������� ���� �� ��������� ������� ��� ������.���������� ������� �����
������������ ���� �� ������� ��������, ����� ���������� ��������� ������� �������,
� ����� ������������ ������� GetOverlappedResult, ����� ���������� ����������
�������� WaitCommEvent.