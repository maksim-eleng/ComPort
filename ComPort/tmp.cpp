

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

Не показан SetCommMask(), поэтому я предполагаю, что маска устанавливается
правильно перед первым вызовом WaitCommEvent(), который обновляет маску,
что делает его некорректным для следующего вызова WaitCommMask().
Попробуйте поместить SetCommMask() в цикл перед WaitCommEvent().

Проблема в том, что WaitCommEvent вызывается с
та же структура ПЕРЕКРЫТИЯ, прежде чем система
завершил предыдущий перекрытый ввод / вывод.

В случае ERROR_IO_PENDING код должен использовать
a WaitForSingleObject, WaitForMultipleObjects или
GetOverlappedResult, чтобы объект OVERLAPPED
Член hEvent получает сигнал перед вызовом
WaitCommEvent снова.

Я приложил пример, который должен делать то, что вы пытаетесь
выполнить.Я скомпилировал его как консольное приложение Win32.

DwEvtMask при использовании перекрывающегося ввода - вывода не должен
дотронуться до завершения ввода / вывода.Это значение будет обновлено
ОС асинхронно.Вызов GetCommMask в
Ваш образец не нужен.

В примере кода используется один поток для открытия COM1 и
другой для проверки ввода с клавиатуры.Нажатие клавиши будет
Сигнал последовательного потока прекратить.


Если перекрывающаяся операция не может быть завершена немедленно, функция
возвращает FALSE, а функция GetLastError возвращает ERROR_IO_PENDING, указывая,
что операция выполняется в фоновом режиме.Когда это происходит, система
устанавливает элемент hEvent структуры OVERLAPPED в состояние без сигнализации
до возврата WaitCommEvent, а затем устанавливает его в состояние сигнализации,
когда происходит одно из указанных событий или ошибка.Вызывающий процесс может
использовать одну из функций ожидания, чтобы определить состояние объекта события,
а затем использовать функцию GetOverlappedResult, чтобы определить результаты
операции WaitCommEvent.