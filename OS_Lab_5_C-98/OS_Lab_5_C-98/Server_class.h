#include "../Employee.h"
#include <Windows.h>
#include <iostream>
#include <string>
#include <string.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <charconv>
#include <tchar.h>

constexpr int StandartSTRSize = 50;
constexpr int BigSTRSize = 150;

constexpr int WaitForOneClientTime = 50;
constexpr int ServerCycleTime = 250;

constexpr int PipeBufferSize = 256;

constexpr char read = 'r';
constexpr char modify = 'm';
constexpr char file = 'f';
constexpr char console = 'c';

#pragma warning (disable:4996)

class Manager; // declaration

// State DP hierarchy:
class StateAbstract;
	class StateBegin;
	class StateRead;
	class StateModify;
		class StateWaitForModifyAccess;
		class StateWrite;
	class StateEnd;

class Server
{
	friend class StateAbstract;
	friend class StateBegin;
	friend class StateRead;
	friend class StateModify;
	friend class StateWrite;
	friend class StateEnd;
	friend class StateWaitForModifyAccess;

	int recordEmount;
	int* record_access;

	FILE* database;
	LPSTR file_name;

	int clientEmount;
	Manager* client_managers;

	void readRecord(int recordNum, employee& result);

	void overrideRecord(int recordNum, employee const& newRecord);

public:

	Server();

	~Server();

	bool CreateDataBase(const char* _filename, int _emount, char mode, LPCSTR input_filename = nullptr);

	bool CreateClients(int _emount);

	int find_by_number(int number);

	void Work();

	void OutputDataBase();

	void InputDataBaseConsole(int _emount);

	void InputDataBaseFile(LPCSTR filename);
};

class Manager
{
	friend class StateAbstract;
	friend class StateBegin;
	friend class StateRead;
	friend class StateModify;
	friend class StateWrite;
	friend class StateEnd;
	friend class StateWaitForModifyAccess;

	int clientNumber;
	_STARTUPINFOA client_StartInf;
	_PROCESS_INFORMATION client_PrInf;
	LPSTR command_line;
	bool works;

	HANDLE communication_pipe;
	LPSTR pipe_name;
	DWORD some_buffer;

	HANDLE clientHasData;
	LPSTR event_name;

	Server* serv;
	StateAbstract* state;

public:

	Manager();

	~Manager();

	bool create(int number, Server* st);

	bool is_working() const;

	void Manage();
};

class StateAbstract 
{
protected:
	Manager* owner;

public:
	explicit StateAbstract(Manager* _m): owner(_m) {}

	virtual ~StateAbstract() = default;

	virtual void action() = 0;
};

class StateBegin: public StateAbstract
{

public:
	using StateAbstract::StateAbstract;

	~StateBegin() final = default;

	void action() override;
};

class StateRead: public StateAbstract
{

public:
	using StateAbstract::StateAbstract;

	~StateRead() final = default;

	void action() override;
};

class StateModify : public StateAbstract
{
protected:
	int buffer;
public:
	StateModify(Manager* _m, int index = -1) : StateAbstract(_m), buffer(index) {}

	~StateModify() override = default;

	void action() override;
};

class StateWaitForModifyAccess : public StateModify
{
public:
	using StateModify::StateModify;

	~StateWaitForModifyAccess() final = default;

	void action() override;
};

class StateWrite : public StateModify
{
public:
	using StateModify::StateModify;

	~StateWrite() final = default;

	void action() override;
};

class StateEnd : public StateAbstract
{
public:
	using StateAbstract::StateAbstract;

	~StateEnd() final = default;

	void action() override;
};