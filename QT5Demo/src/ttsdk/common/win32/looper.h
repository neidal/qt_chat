/*
 * Copyright (C) 2014 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef __LOOPER_H_
#define __LOOPER_H_

//#include <pthread.h>
#include "lock.h"

struct loopermessage{
	unsigned long long what;
	void* obj;
	loopermessage* next;
	bool flag;
};

typedef void (* release_messageobj_callback)(void*);
static void OnDefaultReleaseMessageObj(void* obj)
{
	delete[] (char*)obj;
}

struct loopermessagelist{
	loopermessage* head;
	loopermessage* tail;
	int count;
	release_messageobj_callback rcallback;

	loopermessagelist()
		: head(NULL), tail(NULL), count(0), rcallback(NULL){}

	~loopermessagelist() {
		clear();
	}

	int push(loopermessage* msg) {
		if (!head)
			head = msg;

		if (!tail) {
			tail = msg;
		}
		else {
			tail->next = msg;
			tail = msg;
		}
		++count;
		return count;
	}

	loopermessage* pop() {
		loopermessage* msg = head;
		if (head) {
			head = msg->next;
			msg->next = NULL;
			--count;
		}

		if (!head) {
			tail = NULL;
			count = 0;
		}
		return msg;
	}

	void clear() {
		loopermessage* h = NULL;
		while (h = head) {
			head = head->next;
			if (rcallback && h->obj)
				rcallback(h->obj);
			delete h;
		}
		head = NULL;
		tail = NULL;
		count = 0;
	}
};

class looper {
    public:
        looper();

		void setlistcallback(release_messageobj_callback rcallback);
		void post(int what, void *data, bool flush = false);
        void quit();

	public:
		virtual ~looper();
		virtual void handle(int what, void *data);

    private:
		static DWORD WINAPI trampoline(LPVOID p);
		void addmsg(loopermessage *msg, bool flush);
        void loop();
		loopermessagelist msglist;
		HANDLE worker;
        OFC::CSemaphore headwriteprotect;
        OFC::CWaitSemaphore headdataavailable;
        //sem_t headwriteprotect;
        //sem_t headdataavailable;
        bool running;
};

#endif //__LOOPER_H_
