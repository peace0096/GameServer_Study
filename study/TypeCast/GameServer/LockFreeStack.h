#pragma once

// -------------------
//		1�� �õ�(�̱۽����� ����)
// -------------------


/*

struct SListEntry
{
	SListEntry* next;
};


// [data][ ][ ][ ]
// Header[ next ]

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);

*/

// -------------------
//		2�� �õ�(��Ƽ������ ����)
// -------------------
/*

struct SListEntry
{
	SListEntry* next;
};


// [data][ ][ ][ ]
// Header[ next ]

struct SListHeader
{
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
*/

// -------------------
//		3�� �õ�(ABA Problem �ذ�)
// -------------------

// �����Ϸ����� 16����Ʈ ������ �ش޶�� ��û.
DECLSPEC_ALIGN(16)
struct SListEntry
{
	SListEntry* next;
};

DECLSPEC_ALIGN(16)
struct SListHeader
{
	SListHeader()
	{
		alignment = 0;
		region = 0;
	}

	union
	{
		// 128 ����Ʈ ���
		struct
		{
			uint64 alignment;
			uint64 region;
		} DUMMYSTRUCTNAME;	// �ƹ� �ǹ� ���� define
		// �� 128 ����Ʈ ���
		struct
		{
			uint64 depth : 16;
			uint64 sequence : 48;
			uint64 reserved : 4;
			uint64 next : 60;
		} HeaderX64;
	};
	SListEntry* next = nullptr;
};

void InitializeHead(SListHeader* header);
void PushEntrySList(SListHeader* header, SListEntry* entry);
SListEntry* PopEntrySList(SListHeader* header);
