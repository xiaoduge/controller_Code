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

#ifndef PARSER_H_
#define PARSER_H_

#ifdef __cplusplus
extern "C"
{
#endif

#define T_EOF     0
#define T_TEXT    1
#define T_STRING  2  /* text include in " " */
#define T_NEWLINE 3

struct parse_state
{
    char *ptr;
    char *text;
    int line;
    int nexttoken;
    void *context;
    void *priv;
};

int lookup_keyword(const char *s);
void DUMP(void);
int next_token(struct parse_state *state);
void parse_error(struct parse_state *state, const char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif /* PARSER_H_ */
