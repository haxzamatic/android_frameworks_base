/*
 * Copyright (C) 2010 The Android Open Source Project
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

#ifndef _MTP_SERVER_H
#define _MTP_SERVER_H

#include "MtpRequestPacket.h"
#include "MtpDataPacket.h"
#include "MtpResponsePacket.h"
#include "MtpEventPacket.h"
#include "mtp.h"
#include "MtpUtils.h"

#include <utils/threads.h>

namespace android {

class MtpDatabase;
class MtpStorage;

class MtpServer {

private:
    // file descriptor for MTP kernel driver
    int                 mFD;

    MtpDatabase*        mDatabase;

    // group to own new files and folders
    int                 mFileGroup;
    // permissions for new files and directories
    int                 mFilePermission;
    int                 mDirectoryPermission;

    // current session ID
    MtpSessionID        mSessionID;
    // true if we have an open session and mSessionID is valid
    bool                mSessionOpen;

    MtpRequestPacket    mRequest;
    MtpDataPacket       mData;
    MtpResponsePacket   mResponse;
    MtpEventPacket      mEvent;

    MtpStorageList      mStorages;

    // handle for new object, set by SendObjectInfo and used by SendObject
    MtpObjectHandle     mSendObjectHandle;
    MtpObjectFormat     mSendObjectFormat;
    MtpString           mSendObjectFilePath;
    size_t              mSendObjectFileSize;

    Mutex               mMutex;

public:
                        MtpServer(int fd, MtpDatabase* database,
                                    int fileGroup, int filePerm, int directoryPerm);
    virtual             ~MtpServer();

    MtpStorage*         getStorage(MtpStorageID id);
    inline bool         hasStorage() { return mStorages.size() > 0; }
    bool                hasStorage(MtpStorageID id);
    void                addStorage(MtpStorage* storage);
    void                removeStorage(MtpStorage* storage);

    void                run();

    void                sendObjectAdded(MtpObjectHandle handle);
    void                sendObjectRemoved(MtpObjectHandle handle);

private:
    void                sendStoreAdded(MtpStorageID id);
    void                sendStoreRemoved(MtpStorageID id);
    void                sendEvent(MtpEventCode code, uint32_t param1);

    bool                handleRequest();

    MtpResponseCode     doGetDeviceInfo();
    MtpResponseCode     doOpenSession();
    MtpResponseCode     doCloseSession();
    MtpResponseCode     doGetStorageIDs();
    MtpResponseCode     doGetStorageInfo();
    MtpResponseCode     doGetObjectPropsSupported();
    MtpResponseCode     doGetObjectHandles();
    MtpResponseCode     doGetNumObjects();
    MtpResponseCode     doGetObjectReferences();
    MtpResponseCode     doSetObjectReferences();
    MtpResponseCode     doGetObjectPropValue();
    MtpResponseCode     doSetObjectPropValue();
    MtpResponseCode     doGetDevicePropValue();
    MtpResponseCode     doSetDevicePropValue();
    MtpResponseCode     doResetDevicePropValue();
    MtpResponseCode     doGetObjectPropList();
    MtpResponseCode     doGetObjectInfo();
    MtpResponseCode     doGetObject();
    MtpResponseCode     doGetPartialObject();
    MtpResponseCode     doSendObjectInfo();
    MtpResponseCode     doSendObject();
    MtpResponseCode     doDeleteObject();
    MtpResponseCode     doGetObjectPropDesc();
    MtpResponseCode     doGetDevicePropDesc();
};

}; // namespace android

#endif // _MTP_SERVER_H