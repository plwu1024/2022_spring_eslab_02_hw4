/* Copyright (c) 2015-2020 ARM Limited
 * SPDX-License-Identifier: Apache-2.0
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */
#ifndef BLE_CLIAPP_COMMAND_ARGS_DESCRIPTION_H_
#define BLE_CLIAPP_COMMAND_ARGS_DESCRIPTION_H_

/**
 * @brief simple description of an argument of a command
 */
struct CommandArgDescription {
#if defined(ENABLE_COMMAND_ARG_DESCRIPTION)
    const char* type;                                /// The type of the argument 
    const char* name;                                /// The name of the argument
    const char* desc;                                /// The description of the argument
#else
    const bool type:1;                               /// The type of the argument 
    const bool name:1;                               /// The name of the argument
    const bool desc:1;                               /// The description of the argument
#endif
};

#endif //BLE_CLIAPP_COMMAND_ARGS_DESCRIPTION_H_
