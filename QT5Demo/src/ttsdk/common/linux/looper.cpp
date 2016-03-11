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

#include "looper.h"

void* looper::trampoline(void* p) {
    ((looper*)p)->loop();
    return NULL;
}

looper::looper()
{
//#ifdef __APPLE__
//	char semname[64];
//	int len = sprintf(semname, "looper_data_available_%u", (ULONG_PTR)(&headdataavailable));
//	semname[len] = 0;
//	headdataavailable.Open(semname);
//
//	len = sprintf(semname, "looper_write_protect_%u", (ULONG_PTR)(&headwriteprotect));
//	semname[len] = 0;
//	headwriteprotect.Open(semname);
//#endif
	headdataavailable.Create(false, false);
    pthread_attr_t attr;
    pthread_attr_init(&attr);
	running = true;
    pthread_create(&worker, &attr, trampoline, this);
}

looper::~looper() {
    quit();
}

void looper::setlistcallback(release_messageobj_callback rcallback){
	msglist.rcallback = rcallback;
}

void looper::post(int what, void *data, bool flush) {
    loopermessage *msg = new loopermessage();
    msg->what = what;
	msg->obj = data;
	msg->next = NULL;
	msg->flag = false;
    addmsg(msg, flush);
}

void looper::addmsg(loopermessage *msg, bool flush) {
    //sem_wait(&headwriteprotect);
    headwriteprotect.Lock();
	if (flush)
		msglist.clear();
	msglist.push(msg);
    headwriteprotect.Unlock();
    headdataavailable.SetEvent();
}

void looper::loop() {
	while (running) {
        // wait for available message
        headdataavailable.WaitForEvent();

		while (running)
		{
			// get next available message
			headwriteprotect.Lock();
			loopermessage* msg = msglist.pop();
			headwriteprotect.Unlock();

			if (NULL == msg)
				break;

			if (msg->flag) {
				delete msg;
				return;
			}
			handle(msg->what, msg->obj);
			delete msg;
		}
    }
}

void looper::quit() {
	if (running) {
		loopermessage *msg = new loopermessage();
		msg->what = 0;
		msg->obj = NULL;
		msg->next = NULL;
		msg->flag = true;
		addmsg(msg, true);
		void* retval;
		pthread_join(worker, &retval);
	}
    //headdataavailable.Close();
    //headwriteprotect.Destroy();
    running = false;
}

void looper::handle(int what, void* obj) {
}

