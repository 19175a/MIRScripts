#pragma once

//����������ֹͣ������
class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();

public:

	//�������̣߳�information������ʼ�ĵط�
	void Start(int id = -1);

	//ֹͣ
	void Stop(int id = -1);

	//��ͣ
	void Resume(int id = -1);
	void Suspend(int id = -1);
private:
	tagTaskInfo* GetInfo(int id);
};

