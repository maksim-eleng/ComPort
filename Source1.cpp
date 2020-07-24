
class Parcer
{
public:
	Parcer(std::vector<ComPort>& com, TimeBase& sysClk)
	{
		com.reserve(SysConst::UARTChannel);
		for (int i = 0; i < com.capacity(); ++i) {
			// создаютс€ буферы внутри конструктора ComPort динамически по 512 байт
			// в конструкторах создани€ буфера
			com.push_back({sysClk});
			//здесь вызываютс€ деструкторы ComPort, буфера
			// в векторе - все ок.
			// т.е. создалс€ временный объект, скопировалс€ в вектор и удалилс€
			// ¬опрос: пам€ть передана назад системе? ћожно ли пользоватс€
		}

	}

};


std::vector<ComPort>com;
Parcer parcer(com, sysClk);

void main()
{
	com[0] << "Hellow"; // передача нормальна€, но будет ли так всегда
}

