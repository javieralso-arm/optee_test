/*
 * Copyright (c) 2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <tee_ta_api.h>
#include <user_ta_header_defines.h>
#include <ta_tpm_log.h>
#include <tee_internal_api_extensions.h>
#include <stdio.h>
#include <string.h>

#define LOG_SIZE 1024

/*
 * Trusted Application Entry Points
 */

/* Called each time a new instance is created */
TEE_Result TA_CreateEntryPoint(void)
{
	return TEE_SUCCESS;
}

/* Called each time an instance is destroyed */
void TA_DestroyEntryPoint(void)
{
}

/* Called each time a session is opened */
TEE_Result TA_OpenSessionEntryPoint(uint32_t nParamTypes, TEE_Param pParams[4],
				    void **ppSessionContext)
{
	(void)nParamTypes;
	(void)pParams;
	(void)ppSessionContext;
	return TEE_SUCCESS;
}

/* Called each time a session is closed */
void TA_CloseSessionEntryPoint(void *pSessionContext)
{
	(void)pSessionContext;
}

/* Called when a command is invoked */
TEE_Result TA_InvokeCommandEntryPoint(void *pSessionContext,
				      uint32_t nCommandID, uint32_t nParamTypes,
				      TEE_Param pParams[4])
{
    unsigned char buf[LOG_SIZE];
	size_t len = LOG_SIZE;

	(void)pParams;
	(void)pSessionContext;

	switch (nCommandID) {
	case TA_TPM_LOG_TEST_CMD_GET_LOG:
	/*
	 * Test case: Right arguments.
	 */
		if(TEE_TpmGetEventLog(buf, &len) == 0)
		{
			uint32_t *fieldptr = NULL;
			unsigned char *digestptr = NULL;
			unsigned int i;

			MSG("Parsing the event log header");
			fieldptr = (uint32_t *)&buf[0];

			MSG("--> PCRIndex = 0x%.4x", *fieldptr);
			if(*fieldptr != 0)
			{
				EMSG("Unexpected PCRIndex value. Should be 0x00000000");
				return TEE_ERROR_GENERIC;
			}

			fieldptr = (uint32_t *)&buf[4];
			MSG("--> EventType = 0x%.4x", *fieldptr);
			if(*fieldptr != 3)
			{
				EMSG("Unexpected EventType value. Should be 0x00000003");
				return TEE_ERROR_GENERIC;
			}

			digestptr = &buf[8];
			for(i = 0; i < 20; i++)
			{
				if(*(digestptr + i) != 0)
				{
					EMSG("Digest byte at position %i has unexpected value. Should be 0x0", i);
					return TEE_ERROR_GENERIC;
				}
			}
			MSG("--> Digest value passed");
			MSG("Test passed");
			return TEE_SUCCESS;
		}
		return TEE_ERROR_ACCESS_DENIED;

	case TA_TPM_LOG_TEST_CMD_NULL_BUF:
	/*
	 * Test case: Invalid buffer.
	 */
		len = LOG_SIZE;
		if(TEE_TpmGetEventLog(NULL, &len) == 0)
		{
	    	return TEE_ERROR_GENERIC;
    	}
		return TEE_SUCCESS;

	case TA_TPM_LOG_TEST_CMD_INS_SPACE:
	/*
	 * Test case: Insufficient space on the buffer.
	 */
		len = 1;

		memset((void *)buf, 0, LOG_SIZE);
		if(TEE_TpmGetEventLog(buf, &len) == 0)
		{
			return TEE_ERROR_GENERIC;
		}

		for(unsigned int i = 0; i < LOG_SIZE; i++)
		{
			if(buf[i] != 0)
			{
				EMSG("Buf is expected to be all zeroed");
				return TEE_ERROR_GENERIC;
			}
		}
		return TEE_SUCCESS;

	default:
		return TEE_ERROR_BAD_PARAMETERS;
	}
}
