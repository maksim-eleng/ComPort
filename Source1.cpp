
class Parcer
{
public:
	Parcer(std::vector<ComPort>& com, TimeBase& sysClk)
	{
		com.reserve(SysConst::UARTChannel);
		for (int i = 0; i < com.capacity(); ++i) {
			// ��������� ������ ������ ������������ ComPort ����������� �� 512 ����
			// � ������������� �������� ������
			com.push_back({sysClk});
			//����� ���������� ����������� ComPort, ������
			// � ������� - ��� ��.
			// �.�. �������� ��������� ������, ������������ � ������ � ��������
			// ������: ������ �������� ����� �������? ����� �� �����������
		}

	}

};


std::vector<ComPort>com;
Parcer parcer(com, sysClk);

void main()
{
	com[0] << "Hellow"; // �������� ����������, �� ����� �� ��� ������
}

