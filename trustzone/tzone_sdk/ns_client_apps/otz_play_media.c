/* 
 * OpenVirtualization: 
 * For additional details and support contact developer@sierraware.com.
 * Additional documentation can be found at www.openvirtualization.org
 * 
 * Copyright (C) 2011 SierraWare
 * 
 * This library is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.
 * 
 * Application to send ack signal to hypervisor
 */


#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>
#include "otz_id.h"
#include "otz_tee_client_api.h"

/**
* @brief Sample application to show the implementation of Global Platform Client
* API specification.
*
* @return 
*/
// 논 시큐어 어플리케이션
int main(void)
{
	// 선언
    TEEC_Context context;
    TEEC_Session session;
    TEEC_Operation operation;

    TEEC_Result result;

	// 서비스 아이디
    TEEC_UUID svc_id = OTZ_SVC_VIRTUAL_KEYBOARD;

    uint32_t len;
    char    testData[256];

	// 디바이스 드라이버 오픈 및 컨택스트 초기화
    result = TEEC_InitializeContext(
               NULL,
               &context);

    if(result != TEEC_SUCCESS) {
        goto cleanup_1;
    }

	// tz driver 를 통해서 세션을 연다.
    result = TEEC_OpenSession(
                &context,
                &session,
                &svc_id,
				// PUBLIC 은 체크를 안하는듯.
                TEEC_LOGIN_PUBLIC,
                NULL,
                NULL,
                NULL);

    if(result != TEEC_SUCCESS) {
        goto cleanup_2;
    }

    printf("session id 0x%x\n", session.session_id);

	// 0001 0001 0001 0001
    operation.paramTypes = TEEC_PARAM_TYPES(
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE,
        TEEC_NONE);
    
	// 나중에 보면안다. 오퍼레이션 취소 확인용
    operation.started = 1;

    result = TEEC_InvokeCommand(
                 &session,
                 OTZ_VIRTUAL_CMD_ID_PLAY_MEDIA,
                 &operation,
                 NULL);
    if (result != TEEC_SUCCESS)
    {
        goto cleanup_3;
    }

    printf("command success\n");
cleanup_3:
    TEEC_CloseSession(&session);
cleanup_2:
    TEEC_FinalizeContext(&context);
cleanup_1:
    return 0;
}
