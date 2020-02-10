/*
 * Copyright (c) 2020, ARM Limited and Contributors. All rights reserved.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#ifndef _TA_TPM_LOG_TEST_
#define _TA_TPM_LOG_TEST_

/* This UUID is generated with the ITU-T UUID generator at
   http://www.uuidgenerator.net */
#define TA_TPM_LOG_TEST_UUID { 0xee90d523, 0x90ad, 0x46a0,          \
	{  0x85, 0x9d, 0x8e, 0xea, 0x0b, 0x15, 0x00, 0x86 } }

#define TA_TPM_LOG_TEST_CMD_GET_LOG                 1
#define TA_TPM_LOG_TEST_CMD_NULL_BUF                2
#define TA_TPM_LOG_TEST_CMD_INS_SPACE               3

#endif /* _TA_TPM_LOG_TEST_ */
