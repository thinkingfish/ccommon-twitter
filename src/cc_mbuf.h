/*
 * ccommon - a cache common library.
 * Copyright (C) 2013 Twitter, Inc.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 * http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#ifndef _CC_MBUF_H_
#define _CC_MBUF_H_

#include <stdint.h>
#include <stdlib.h>

typedef void (*mbuf_copy_t)(struct mbuf *, void *);

struct mbuf {
    uint32_t           magic;   /* mbuf magic (const) */
    STAILQ_ENTRY(mbuf) next;    /* next mbuf */
    uint8_t            *rpos;   /* read marker */
    uint8_t            *wpos;   /* write marker */
    uint8_t            *start;  /* start of buffer (const) */
    uint8_t            *end;    /* end of buffer (const) */
};

STAILQ_HEAD(mq, mbuf);

#define MBUF_MAGIC      0xdeadbeef
#define MBUF_MIN_SIZE   512
#define MBUF_MAX_SIZE   65536
#define MBUF_SIZE       16384
#define MBUF_HDR_SIZE   sizeof(struct mbuf)

static inline bool
mbuf_empty(struct mbuf *mbuf)
{
    return mbuf->rpos == mbuf->wpos ? true : false;
}

static inline bool
mbuf_full(struct mbuf *mbuf)
{
    return mbuf->wpos == mbuf->end ? true : false;
}

void mbuf_init(size_t chunk_size);
void mbuf_deinit(void);
struct mbuf *mbuf_get(void);
void mbuf_put(struct mbuf *mbuf);
void mbuf_reset(struct mbuf *mbuf);
uint32_t mbuf_rsize(struct mbuf *mbuf);
uint32_t mbuf_wsize(struct mbuf *mbuf);
size_t mbuf_capacity(void);
void mbuf_insert(struct mq *mq, struct mbuf *mbuf);
void mbuf_remove(struct mq *mq, struct mbuf *mbuf);
void mbuf_copy(struct mbuf *mbuf, uint8_t *rpos, size_t n);
struct mbuf *mbuf_split(struct mq *h, uint8_t *rpos, mbuf_copy_t cb, void *cbarg);

#endif
