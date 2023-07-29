#pragma once

//任务：启动，停止，结束
class CTaskManager
{
public:
	CTaskManager();
	~CTaskManager();

public:

	//启动：线程，information，程序开始的地方
	void Start(int id = -1);

	//停止
	void Stop(int id = -1);

	//暂停
	void Resume(int id = -1);
	void Suspend(int id = -1);
private:
	tagTaskInfo* GetInfo(int id);
};

