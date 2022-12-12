typedef struct {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[64];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;
typedef struct {
  uint32_t key[8];
  blake3_chunk_state chunk;
  uint8_t cv_stack_len;
  uint8_t cv_stack[(54 + 1) * 32];
} blake3_hasher;
void blake3_hasher_init(blake3_hasher *self);
void blake3_hasher_init_keyed(blake3_hasher *self,
                              const uint8_t key[32]);
void blake3_hasher_init_derive_key(blake3_hasher *self, const char *context);
void blake3_hasher_update(blake3_hasher *self, const void *input,
                          size_t input_len);
void blake3_hasher_finalize(const blake3_hasher *self, uint8_t *out,
                            size_t out_len);
void blake3_hasher_finalize_seek(const blake3_hasher *self, uint64_t seek,
                                 uint8_t *out, size_t out_len);
enum blake3_flags {
  CHUNK_START = 1 << 0,
  CHUNK_END = 1 << 1,
  PARENT = 1 << 2,
  ROOT = 1 << 3,
  KEYED_HASH = 1 << 4,
  DERIVE_KEY_CONTEXT = 1 << 5,
  DERIVE_KEY_MATERIAL = 1 << 6,
};
static const uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};
static const uint8_t MSG_SCHEDULE[7][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {2, 6, 3, 10, 7, 0, 4, 13, 1, 11, 12, 5, 9, 14, 15, 8},
    {3, 4, 10, 12, 13, 2, 7, 14, 6, 5, 9, 0, 11, 15, 8, 1},
    {10, 7, 12, 9, 14, 3, 13, 15, 4, 0, 11, 2, 5, 8, 1, 6},
    {12, 13, 9, 11, 15, 10, 14, 8, 7, 2, 5, 3, 0, 1, 6, 4},
    {9, 14, 11, 5, 8, 12, 15, 1, 13, 3, 0, 10, 2, 6, 4, 7},
    {11, 15, 5, 0, 1, 9, 8, 6, 14, 10, 2, 12, 3, 4, 7, 13},
};
static unsigned int highest_one(uint64_t x) {
  return 63 ^ __builtin_clzll(x);
}
static unsigned int popcnt(uint64_t x) {
  unsigned int count = 0;
  while (x != 0) {
    count += 1;
    x &= x - 1;
  }
  return count;
}
static uint64_t round_down_to_power_of_2(uint64_t x) {
  return 1ULL << highest_one(x | 1);
}
static uint32_t counter_low(uint64_t counter) { return (uint32_t)counter; }
static uint32_t counter_high(uint64_t counter) {
  return (uint32_t)(counter >> 32);
}
static uint32_t load32(const void *src) {
  return(*((uint*)src));
}
static void load_key_words(const uint8_t key[32],
                           uint32_t key_words[8]) {
  key_words[0] = load32(&key[0 * 4]);
  key_words[1] = load32(&key[1 * 4]);
  key_words[2] = load32(&key[2 * 4]);
  key_words[3] = load32(&key[3 * 4]);
  key_words[4] = load32(&key[4 * 4]);
  key_words[5] = load32(&key[5 * 4]);
  key_words[6] = load32(&key[6 * 4]);
  key_words[7] = load32(&key[7 * 4]);
}
void blake3_compress_in_place(uint32_t cv[8],
                              const uint8_t block[64],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags);
void blake3_compress_xof(const uint32_t cv[8],
                         const uint8_t block[64],
                         uint8_t block_len, uint64_t counter, uint8_t flags,
                         uint8_t out[64]);
void blake3_hash_many(const uint8_t *const *inputs, size_t num_inputs,
                      size_t blocks, const uint32_t key[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_end, uint8_t *out);
size_t blake3_simd_degree(void);
void blake3_compress_in_place_portable(uint32_t cv[8],
                                       const uint8_t block[64],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags);
void blake3_compress_xof_portable(const uint32_t cv[8],
                                  const uint8_t block[64],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]);
void blake3_hash_many_portable(const uint8_t *const *inputs, size_t num_inputs,
                               size_t blocks, const uint32_t key[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out);
static void chunk_state_init(blake3_chunk_state *self, const uint32_t key[8],
                             uint8_t flags) {
  memcpy(self->cv, key, 32);
  self->chunk_counter = 0;
  memset(self->buf, 0, 64);
  self->buf_len = 0;
  self->blocks_compressed = 0;
  self->flags = flags;
}
static void chunk_state_reset(blake3_chunk_state *self, const uint32_t key[8],
                              uint64_t chunk_counter) {
  memcpy(self->cv, key, 32);
  self->chunk_counter = chunk_counter;
  self->blocks_compressed = 0;
  memset(self->buf, 0, 64);
  self->buf_len = 0;
}
static size_t chunk_state_len(const blake3_chunk_state *self) {
  return (64 * (size_t)self->blocks_compressed) +
         ((size_t)self->buf_len);
}
static size_t chunk_state_fill_buf(blake3_chunk_state *self,
                                   const uint8_t *input, size_t input_len) {
  size_t take = 64 - ((size_t)self->buf_len);
  if (take > input_len) {
    take = input_len;
  }
  uint8_t *dest = self->buf + ((size_t)self->buf_len);
  memcpy(dest, input, take);
  self->buf_len += (uint8_t)take;
  return take;
}
static uint8_t chunk_state_maybe_start_flag(const blake3_chunk_state *self) {
  if (self->blocks_compressed == 0) {
    return CHUNK_START;
  } else {
    return 0;
  }
}
typedef struct {
  uint32_t input_cv[8];
  uint64_t counter;
  uint8_t block[64];
  uint8_t block_len;
  uint8_t flags;
} output_t;
static output_t make_output(const uint32_t input_cv[8],
                            const uint8_t block[64],
                            uint8_t block_len, uint64_t counter,
                            uint8_t flags) {
  output_t ret;
  memcpy(ret.input_cv, input_cv, 32);
  memcpy(ret.block, block, 64);
  ret.block_len = block_len;
  ret.counter = counter;
  ret.flags = flags;
  return ret;
}
static void output_chaining_value(const output_t *self, uint8_t cv[32]) {
  uint32_t cv_words[8];
  memcpy(cv_words, self->input_cv, 32);
  blake3_compress_in_place(cv_words, self->block, self->block_len,
                           self->counter, self->flags);
  memcpy(cv, cv_words, 32);
}
static void output_root_bytes(const output_t *self, uint64_t seek, uint8_t *out,
                              size_t out_len) {
  uint64_t output_block_counter = seek / 64;
  size_t offset_within_block = seek % 64;
  uint8_t wide_buf[64];
  while (out_len > 0) {
    blake3_compress_xof(self->input_cv, self->block, self->block_len,
                        output_block_counter, self->flags | ROOT, wide_buf);
    size_t available_bytes = 64 - offset_within_block;
    size_t memcpy_len;
    if (out_len > available_bytes) {
      memcpy_len = available_bytes;
    } else {
      memcpy_len = out_len;
    }
    memcpy(out, wide_buf + offset_within_block, memcpy_len);
    out += memcpy_len;
    out_len -= memcpy_len;
    output_block_counter += 1;
    offset_within_block = 0;
  }
}
static void chunk_state_update(blake3_chunk_state *self, const uint8_t *input,
                               size_t input_len) {
  if (self->buf_len > 0) {
    size_t take = chunk_state_fill_buf(self, input, input_len);
    input += take;
    input_len -= take;
    if (input_len > 0) {
      blake3_compress_in_place(
          self->cv, self->buf, 64, self->chunk_counter,
          self->flags | chunk_state_maybe_start_flag(self));
      self->blocks_compressed += 1;
      self->buf_len = 0;
      memset(self->buf, 0, 64);
    }
  }
  while (input_len > 64) {
    blake3_compress_in_place(self->cv, input, 64,
                             self->chunk_counter,
                             self->flags | chunk_state_maybe_start_flag(self));
    self->blocks_compressed += 1;
    input += 64;
    input_len -= 64;
  }
  size_t take = chunk_state_fill_buf(self, input, input_len);
  input += take;
  input_len -= take;
}
static output_t chunk_state_output(const blake3_chunk_state *self) {
  uint8_t block_flags =
      self->flags | chunk_state_maybe_start_flag(self) | CHUNK_END;
  return make_output(self->cv, self->buf, self->buf_len, self->chunk_counter,
                     block_flags);
}
static output_t parent_output(const uint8_t block[64],
                              const uint32_t key[8], uint8_t flags) {
  return make_output(key, block, 64, 0, flags | PARENT);
}
static size_t left_len(size_t content_len) {
  size_t full_chunks = (content_len - 1) / 1024;
  return round_down_to_power_of_2(full_chunks) * 1024;
}
static size_t compress_chunks_parallel(const uint8_t *input, size_t input_len,
                                       const uint32_t key[8],
                                       uint64_t chunk_counter, uint8_t flags,
                                       uint8_t *out) {
  const uint8_t *chunks_array[16];
  size_t input_position = 0;
  size_t chunks_array_len = 0;
  while (input_len - input_position >= 1024) {
    chunks_array[chunks_array_len] = &input[input_position];
    input_position += 1024;
    chunks_array_len += 1;
  }
  blake3_hash_many(chunks_array, chunks_array_len,
                   1024 / 64, key, chunk_counter,
                   true, flags, CHUNK_START, CHUNK_END, out);
  if (input_len > input_position) {
    uint64_t counter = chunk_counter + (uint64_t)chunks_array_len;
    blake3_chunk_state chunk_state;
    chunk_state_init(&chunk_state, key, flags);
    chunk_state.chunk_counter = counter;
    chunk_state_update(&chunk_state, &input[input_position],
                       input_len - input_position);
    output_t output = chunk_state_output(&chunk_state);
    output_chaining_value(&output, &out[chunks_array_len * 32]);
    return chunks_array_len + 1;
  } else {
    return chunks_array_len;
  }
}
static size_t compress_parents_parallel(const uint8_t *child_chaining_values,
                                        size_t num_chaining_values,
                                        const uint32_t key[8], uint8_t flags,
                                        uint8_t *out) {
  const uint8_t *parents_array[(16 > 2 ? 16 : 2)];
  size_t parents_array_len = 0;
  while (num_chaining_values - (2 * parents_array_len) >= 2) {
    parents_array[parents_array_len] =
        &child_chaining_values[2 * parents_array_len * 32];
    parents_array_len += 1;
  }
  blake3_hash_many(parents_array, parents_array_len, 1, key,
                   0,
                   false, flags | PARENT,
                   0,
                   0,
                   out);
  if (num_chaining_values > 2 * parents_array_len) {
    memcpy(&out[parents_array_len * 32],
           &child_chaining_values[2 * parents_array_len * 32],
           32);
    return parents_array_len + 1;
  } else {
    return parents_array_len;
  }
}
static size_t blake3_compress_subtree_wide(const uint8_t *input,
                                           size_t input_len,
                                           const uint32_t key[8],
                                           uint64_t chunk_counter,
                                           uint8_t flags, uint8_t *out) {
  if (input_len <= blake3_simd_degree() * 1024) {
    return compress_chunks_parallel(input, input_len, key, chunk_counter, flags,
                                    out);
  }
  size_t left_input_len = left_len(input_len);
  size_t right_input_len = input_len - left_input_len;
  const uint8_t *right_input = &input[left_input_len];
  uint64_t right_chunk_counter =
      chunk_counter + (uint64_t)(left_input_len / 1024);
  uint8_t cv_array[2 * (16 > 2 ? 16 : 2) * 32];
  size_t degree = blake3_simd_degree();
  if (left_input_len > 1024 && degree == 1) {
    degree = 2;
  }
  uint8_t *right_cvs = &cv_array[degree * 32];
  size_t left_n = blake3_compress_subtree_wide(input, left_input_len, key,
                                               chunk_counter, flags, cv_array);
  size_t right_n = blake3_compress_subtree_wide(
      right_input, right_input_len, key, right_chunk_counter, flags, right_cvs);
  if (left_n == 1) {
    memcpy(out, cv_array, 2 * 32);
    return 2;
  }
  size_t num_chaining_values = left_n + right_n;
  return compress_parents_parallel(cv_array, num_chaining_values, key, flags,
                                   out);
}
static void compress_subtree_to_parent_node(
    const uint8_t *input, size_t input_len, const uint32_t key[8],
    uint64_t chunk_counter, uint8_t flags, uint8_t out[2 * 32]) {
  uint8_t cv_array[2 * (16 > 2 ? 16 : 2) * 32];
  size_t num_cvs = blake3_compress_subtree_wide(input, input_len, key,
                                                chunk_counter, flags, cv_array);
  uint8_t out_array[(16 > 2 ? 16 : 2) * 32 / 2];
  while (num_cvs > 2) {
    num_cvs =
        compress_parents_parallel(cv_array, num_cvs, key, flags, out_array);
    memcpy(cv_array, out_array, num_cvs * 32);
  }
  memcpy(out, cv_array, 2 * 32);
}
static void hasher_init_base(blake3_hasher *self, const uint32_t key[8],
                             uint8_t flags) {
  memcpy(self->key, key, 32);
  chunk_state_init(&self->chunk, key, flags);
  self->cv_stack_len = 0;
}
void blake3_hasher_init(blake3_hasher *self) { hasher_init_base(self, IV, 0); }
void blake3_hasher_init_keyed(blake3_hasher *self,
                              const uint8_t key[32]) {
  uint32_t key_words[8];
  load_key_words(key, key_words);
  hasher_init_base(self, key_words, KEYED_HASH);
}
void blake3_hasher_init_derive_key(blake3_hasher *self, const char *context) {
  blake3_hasher context_hasher;
  hasher_init_base(&context_hasher, IV, DERIVE_KEY_CONTEXT);
  blake3_hasher_update(&context_hasher, context, strlen(context));
  uint8_t context_key[32];
  blake3_hasher_finalize(&context_hasher, context_key, 32);
  uint32_t context_key_words[8];
  load_key_words(context_key, context_key_words);
  hasher_init_base(self, context_key_words, DERIVE_KEY_MATERIAL);
}
static void hasher_merge_cv_stack(blake3_hasher *self, uint64_t total_len) {
  size_t post_merge_stack_len = (size_t)popcnt(total_len);
  while (self->cv_stack_len > post_merge_stack_len) {
    uint8_t *parent_node =
        &self->cv_stack[(self->cv_stack_len - 2) * 32];
    output_t output = parent_output(parent_node, self->key, self->chunk.flags);
    output_chaining_value(&output, parent_node);
    self->cv_stack_len -= 1;
  }
}
static void hasher_push_cv(blake3_hasher *self, uint8_t new_cv[32],
                           uint64_t chunk_counter) {
  hasher_merge_cv_stack(self, chunk_counter);
  memcpy(&self->cv_stack[self->cv_stack_len * 32], new_cv,
         32);
  self->cv_stack_len += 1;
}
void blake3_hasher_update(blake3_hasher *self, const void *input,
                          size_t input_len) {
  if (input_len == 0) {
    return;
  }
  const uint8_t *input_bytes = (const uint8_t *)input;
  if (chunk_state_len(&self->chunk) > 0) {
    size_t take = 1024 - chunk_state_len(&self->chunk);
    if (take > input_len) {
      take = input_len;
    }
    chunk_state_update(&self->chunk, input_bytes, take);
    input_bytes += take;
    input_len -= take;
    if (input_len > 0) {
      output_t output = chunk_state_output(&self->chunk);
      uint8_t chunk_cv[32];
      output_chaining_value(&output, chunk_cv);
      hasher_push_cv(self, chunk_cv, self->chunk.chunk_counter);
      chunk_state_reset(&self->chunk, self->key, self->chunk.chunk_counter + 1);
    } else {
      return;
    }
  }
  while (input_len > 1024) {
    size_t subtree_len = round_down_to_power_of_2(input_len);
    uint64_t count_so_far = self->chunk.chunk_counter * 1024;
    while ((((uint64_t)(subtree_len - 1)) & count_so_far) != 0) {
      subtree_len /= 2;
    }
    uint64_t subtree_chunks = subtree_len / 1024;
    if (subtree_len <= 1024) {
      blake3_chunk_state chunk_state;
      chunk_state_init(&chunk_state, self->key, self->chunk.flags);
      chunk_state.chunk_counter = self->chunk.chunk_counter;
      chunk_state_update(&chunk_state, input_bytes, subtree_len);
      output_t output = chunk_state_output(&chunk_state);
      uint8_t cv[32];
      output_chaining_value(&output, cv);
      hasher_push_cv(self, cv, chunk_state.chunk_counter);
    } else {
      uint8_t cv_pair[2 * 32];
      compress_subtree_to_parent_node(input_bytes, subtree_len, self->key,
                                      self->chunk.chunk_counter,
                                      self->chunk.flags, cv_pair);
      hasher_push_cv(self, cv_pair, self->chunk.chunk_counter);
      hasher_push_cv(self, &cv_pair[32],
                     self->chunk.chunk_counter + (subtree_chunks / 2));
    }
    self->chunk.chunk_counter += subtree_chunks;
    input_bytes += subtree_len;
    input_len -= subtree_len;
  }
  if (input_len > 0) {
    chunk_state_update(&self->chunk, input_bytes, input_len);
    hasher_merge_cv_stack(self, self->chunk.chunk_counter);
  }
}
void blake3_hasher_finalize(const blake3_hasher *self, uint8_t *out,
                            size_t out_len) {
  blake3_hasher_finalize_seek(self, 0, out, out_len);
}
void blake3_hasher_finalize_seek(const blake3_hasher *self, uint64_t seek,
                                 uint8_t *out, size_t out_len) {
  if (out_len == 0) {
    return;
  }
  if (self->cv_stack_len == 0) {
    output_t output = chunk_state_output(&self->chunk);
    output_root_bytes(&output, seek, out, out_len);
    return;
  }
  output_t output;
  size_t cvs_remaining;
  if (chunk_state_len(&self->chunk) > 0) {
    cvs_remaining = self->cv_stack_len;
    output = chunk_state_output(&self->chunk);
  } else {
    cvs_remaining = self->cv_stack_len - 2;
    output = parent_output(&self->cv_stack[cvs_remaining * 32], self->key,
                           self->chunk.flags);
  }
  while (cvs_remaining > 0) {
    cvs_remaining -= 1;
    uint8_t parent_block[64];
    memcpy(parent_block, &self->cv_stack[cvs_remaining * 32], 32);
    output_chaining_value(&output, &parent_block[32]);
    output = parent_output(parent_block, self->key, self->chunk.flags);
  }
  output_root_bytes(&output, seek, out, out_len);
}
static void store32(void *dst, uint32_t w) {
 *((uint*)dst) = w;
}
static void g(uint32_t *state, size_t a, size_t b, size_t c, size_t d,
              uint32_t x, uint32_t y) {
  state[a] = state[a] + state[b] + x;
  state[d] = ({ uint i = state[d] ^ state[a]; ROR(16,i); i; });
  state[c] = state[c] + state[d];
  state[b] = ({ uint i = state[b] ^ state[c]; ROR(12,i); i; });
  state[a] = state[a] + state[b] + y;
  state[d] = ({ uint i = state[d] ^ state[a]; ROR(8,i); i; });
  state[c] = state[c] + state[d];
  state[b] = ({ uint i = state[b] ^ state[c]; ROR(7,i); i; });
}
static void round_fn(uint32_t state[16], const uint32_t *msg, size_t round) {
  const uint8_t *schedule = MSG_SCHEDULE[round];
  for ( int a = 0; a<4; a++ )
   g(state, a, a+4, a+8, a+12, msg[schedule[a*2]], msg[schedule[a*2+1]]);
  static const uchar mix[] = { 0,5,10,15,1,6,11,12,2,7,8,13,3,4,9,14 };
  for ( int a = 0; a<4; a++ )
   g(state, mix[a*4], mix[a*4+1], mix[a*4+2], mix[a*4+3],msg[schedule[a*2+8]], msg[schedule[a*2+9]]);
}
static void compress_pre(uint32_t state[16], const uint32_t cv[8],
                         const uint8_t block[64],
                         uint8_t block_len, uint64_t counter, uint8_t flags) {
  uint32_t block_words[16];
  for ( int a = 0; a<8; a++ ){
   *((ulong*)block_words + a) = *((ulong*)block + a);
    state[a] = cv[a];
  }
  state[8] = IV[0];
  state[9] = IV[1];
  state[10] = IV[2];
  state[11] = IV[3];
  state[12] = counter_low(counter);
  state[13] = counter_high(counter);
  state[14] = (uint32_t)block_len;
  state[15] = (uint32_t)flags;
  for ( int a = 0; a<7; a++ )
   round_fn(state, &block_words[0], a);
}
void blake3_compress_in_place(uint32_t cv[8],
                                       const uint8_t block[64],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags) {
  uint32_t state[16];
  compress_pre(state, cv, block, block_len, counter, flags);
  for ( int a = 0; a<4; a++ )
    *((ulong*)cv+a) = *((ulong*)state+a) ^ *((ulong*)state+a+4);
}
void blake3_compress_xof(const uint32_t cv[8],
                                  const uint8_t block[64],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]) {
  uint32_t state[16];
  compress_pre(state, cv, block, block_len, counter, flags);
  for ( int a = 0; a<8; a++ ){
   store32(&out[a*4], state[a] ^ state[a+8] );
   store32(&out[(a+8)*4], state[a] ^ cv[a] );
  }
}
static void hash_one(const uint8_t *input, size_t blocks,
                              const uint32_t key[8], uint64_t counter,
                              uint8_t flags, uint8_t flags_start,
                              uint8_t flags_end, uint8_t out[32]) {
  uint32_t cv[8];
  memcpy(cv, key, 32);
  uint8_t block_flags = flags | flags_start;
  while (blocks > 0) {
    if (blocks == 1) {
      block_flags |= flags_end;
    }
    blake3_compress_in_place(cv, input, 64, counter,
                                      block_flags);
    input = &input[64];
    blocks -= 1;
    block_flags = flags;
  }
  memcpy(out, cv, 32);
}
void blake3_hash_many(const uint8_t *const *inputs, size_t num_inputs,
                               size_t blocks, const uint32_t key[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out) {
  while (num_inputs > 0) {
    hash_one(inputs[0], blocks, key, counter, flags, flags_start,
                      flags_end, out);
    if (increment_counter) {
      counter += 1;
    }
    inputs += 1;
    num_inputs -= 1;
    out = &out[32];
  }
}
size_t blake3_simd_degree(void) {
 return 1;
}
MAIN{
 uchar buf[4000];
 uchar obuf[64];
 blake3_hasher ctx;
 int r;
 int blen = 32;
 blake3_hasher_init(&ctx);
 while ( (r=nread(0,(char*)buf,4000)) ){
  blake3_hasher_update(&ctx,buf,r);
 }
 blake3_hasher_finalize(&ctx,obuf,blen);
 for ( int a = 0; a<blen; a++ ){
  char c = obuf[a];
  buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
  c >>=4;
  buf[2*a] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
 }
 buf[blen*2] = '\n';
 write(1,buf,(blen*2+1));
 exit(0);
}
typedef long jmp_buf[8];
typedef unsigned int ml_size_t;
typedef int ml_off_t;
typedef unsigned int uint32_t;
typedef unsigned char uint8_t;
typedef unsigned long size_t;
typedef int bool;
inline void __attribute__((noreturn,always_inline))__ABORT(){
 asm("xor %esp,%esp\nxor %ebp,%ebp\nret");
 __builtin_unreachable();
}
typedef struct _text_match { char *pos; int len; } text_match;
typedef int FILE;
typedef union { FILE *F; int i, fd;} mfl_union;
typedef struct { int quot; int rem; } div_t;
typedef struct { long int quot; long int rem; } ldiv_t;
typedef void (*sighandler_t)(int);
typedef void (functionp)(void);
typedef struct {
  int tz_minuteswest;
  int tz_dsttime;
} timezone;
typedef unsigned long fd_mask;
typedef struct {
 unsigned long fds_bits[1024 / 8 / sizeof(long)];
} fd_set;
typedef __builtin_va_list va_list;
enum EPOLL_EVENTS { __EPOLL_DUMMY };
typedef union epoll_data {
    void *ptr;
     int fd;
    int u32;
    long u64;
} epoll_data_t;
struct epoll_event {
    int events;
     epoll_data_t data;
} __attribute__ ((__packed__));
typedef long time_t;
typedef long suseconds_t;
typedef unsigned useconds_t;
struct timeval { time_t tv_sec; suseconds_t tv_usec; };
struct itimerval {
  struct timeval it_interval;
  struct timeval it_value;
};
typedef unsigned char uchar;
typedef int wchar_t;
typedef long double double_t;
typedef float float_t;
typedef struct { long long __ll; long double __ld; } max_align_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; unsigned long __s[7]; } __u; } pthread_attr_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } pthread_mutex_t;
typedef struct { union { int __i[10]; volatile int __vi[10]; volatile void *volatile __p[5]; } __u; } mtx_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } pthread_cond_t;
typedef struct { union { int __i[12]; volatile int __vi[12]; void *__p[6]; } __u; } cnd_t;
typedef struct { union { int __i[14]; volatile int __vi[14]; void *__p[7]; } __u; } pthread_rwlock_t;
typedef struct { union { int __i[8]; volatile int __vi[8]; void *__p[4]; } __u; } pthread_barrier_t;
typedef unsigned long uintptr_t;
typedef long ptrdiff_t;
typedef long ssize_t;
typedef long intptr_t;
typedef long regoff_t;
typedef long register_t;
typedef signed char int8_t;
typedef signed short int16_t;
typedef signed int int32_t;
typedef signed long int64_t;
typedef signed long intmax_t;
typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long uint64_t;
typedef unsigned long u_int64_t;
typedef unsigned long uintmax_t;
typedef unsigned mode_t;
typedef unsigned long nlink_t;
typedef long off_t;
typedef unsigned long ino_t;
typedef unsigned long dev_t;
typedef long blksize_t;
typedef long blkcnt_t;
typedef unsigned long fsblkcnt_t;
typedef unsigned long fsfilcnt_t;
typedef unsigned wint_t;
typedef unsigned long wctype_t;
typedef void * timer_t;
typedef int clockid_t;
typedef long clock_t;
struct timespec { time_t tv_sec; long tv_nsec; };
typedef int pid_t;
typedef unsigned id_t;
typedef unsigned uid_t;
typedef unsigned gid_t;
typedef int key_t;
typedef struct __mbstate_t { unsigned __opaque1, __opaque2; } mbstate_t;
typedef struct __locale_struct * locale_t;
struct iovec { void *iov_base; long iov_len; };
typedef unsigned socklen_t;
typedef unsigned short sa_family_t;
struct winsize {
 unsigned short ws_row;
 unsigned short ws_col;
 unsigned short ws_xpixel;
 unsigned short ws_ypixel;
};
struct stat {
  dev_t st_dev;
  ino_t st_ino;
  nlink_t st_nlink;
  mode_t st_mode;
  uid_t st_uid;
  gid_t st_gid;
  unsigned int __pad0;
  dev_t st_rdev;
  off_t st_size;
  blksize_t st_blksize;
  blkcnt_t st_blocks;
  struct timespec st_atime;
  struct timespec st_mtime;
  struct timespec st_ctime;
  long __unused[3];
};
int isalnum_l(int, locale_t);
int isalpha_l(int, locale_t);
int isblank_l(int, locale_t);
int iscntrl_l(int, locale_t);
int isdigit_l(int, locale_t);
int isgraph_l(int, locale_t);
int islower_l(int, locale_t);
int isprint_l(int, locale_t);
int ispunct_l(int, locale_t);
int isspace_l(int, locale_t);
int isupper_l(int, locale_t);
int isxdigit_l(int, locale_t);
int tolower_l(int, locale_t);
int toupper_l(int, locale_t);
typedef int8_t int_fast8_t;
typedef int64_t int_fast64_t;
typedef int8_t int_least8_t;
typedef int16_t int_least16_t;
typedef int32_t int_least32_t;
typedef int64_t int_least64_t;
typedef uint8_t uint_fast8_t;
typedef uint64_t uint_fast64_t;
typedef uint8_t uint_least8_t;
typedef uint16_t uint_least16_t;
typedef uint32_t uint_least32_t;
typedef uint64_t uint_least64_t;
typedef unsigned char u_int8_t;
typedef unsigned short u_int16_t;
typedef unsigned u_int32_t;
typedef char *caddr_t;
typedef unsigned char u_char;
typedef unsigned short u_short, ushort;
typedef unsigned u_int, uint;
typedef unsigned long u_long, ulong;
typedef long long quad_t;
typedef unsigned long long u_quad_t;
struct sockaddr {
 sa_family_t sa_family;
 char sa_data[14];
};
struct itimerspec {
 struct timespec it_interval;
 struct timespec it_value;
};
struct oldold_utsname {
        char sysname[9];
        char nodename[9];
        char release[9];
        char version[9];
        char machine[9];
};
struct old_utsname {
        char sysname[65];
        char nodename[65];
        char release[65];
        char version[65];
        char machine[65];
};
struct new_utsname {
        char sysname[64 + 1];
        char nodename[64 + 1];
        char release[64 + 1];
        char version[64 + 1];
        char machine[64 + 1];
        char domainname[64 + 1];
};
typedef long __band_t;
typedef union sigval {
  int sival_int;
  void *sival_ptr;
} sigval_t;
typedef struct siginfo {
  int32_t si_signo;
  int32_t si_errno;
  int32_t si_code;
  union {
    char __pad[128 - 2*sizeof(int) - sizeof(long)];
    struct {
      pid_t _pid;
      uid_t _uid;
    } _kill;
    struct {
      uint32_t _timer1;
      uint32_t _timer2;
    } _timer;
    struct {
      pid_t _pid;
      uid_t _uid;
      sigval_t _sigval;
    } _rt;
    struct {
      pid_t _pid;
      uid_t _uid;
      int32_t _status;
      clock_t _utime;
      clock_t _stime;
    } _sigchld;
    struct {
      void *_addr;
    } _sigfault;
    struct {
      __band_t _band;
      int32_t _fd;
    } _sigpoll;
  } _sifields;
} siginfo_t;
typedef struct __sigset_t { unsigned long sig; } sigset_t;
struct sigaction {
  void (*sa_handler)(int);
  unsigned long sa_flags;
  void (*restorer)(void);
  sigset_t sa_mask;
};
struct prctl_mm_map {
 uint64_t start_code;
 uint64_t end_code;
 uint64_t start_data;
 uint64_t end_data;
 uint64_t start_brk;
 uint64_t brk;
 uint64_t start_stack;
 uint64_t arg_start;
 uint64_t arg_end;
 uint64_t env_start;
 uint64_t env_end;
 uint64_t *auxv;
 uint32_t auxv_size;
 uint32_t exe_fd;
};
struct tms
  {
    clock_t tms_utime;
    clock_t tms_stime;
    clock_t tms_cutime;
    clock_t tms_cstime;
  };
struct inotify_event {
 int wd;
 uint32_t mask, cookie, len;
 char name[];
};
typedef long long __kernel_long_t;
typedef unsigned long __kernel_ulong_t;
typedef int sig_atomic_t;
typedef void (*__sighandler_t) (int);
typedef __sighandler_t sig_t;
typedef unsigned long __u64;
typedef unsigned int __u32;
typedef signed long __s64;
typedef signed int __s32;
typedef unsigned short __u16;
typedef signed short __s16;
struct statx_timestamp{
  int64_t tv_sec;
  uint32_t tv_nsec;
  int32_t __statx_timestamp_pad1[1];
};
struct statx{
  uint32_t stx_mask;
  uint32_t stx_blksize;
  uint64_t stx_attributes;
  uint32_t stx_nlink;
  uint32_t stx_uid;
  uint32_t stx_gid;
  uint16_t stx_mode;
  uint16_t __statx_pad1[1];
  uint64_t stx_ino;
  uint64_t stx_size;
  uint64_t stx_blocks;
  uint64_t stx_attributes_mask;
  struct statx_timestamp stx_atime;
  struct statx_timestamp stx_btime;
  struct statx_timestamp stx_ctime;
  struct statx_timestamp stx_mtime;
  uint32_t stx_rdev_major;
  uint32_t stx_rdev_minor;
  uint32_t stx_dev_major;
  uint32_t stx_dev_minor;
  uint64_t __statx_pad2[14];
};
enum fsconfig_command {
 FSCONFIG_SET_FLAG = 0,
 FSCONFIG_SET_STRING = 1,
 FSCONFIG_SET_BINARY = 2,
 FSCONFIG_SET_PATH = 3,
 FSCONFIG_SET_PATH_EMPTY = 4,
 FSCONFIG_SET_FD = 5,
 FSCONFIG_CMD_CREATE = 6,
 FSCONFIG_CMD_RECONFIGURE = 7,
};
struct seccomp_data {
 int nr;
 __u32 arch;
 __u64 instruction_pointer;
 __u64 args[6];
};
struct seccomp_notif_sizes {
 __u16 seccomp_notif;
 __u16 seccomp_notif_resp;
 __u16 seccomp_data;
};
struct seccomp_notif {
 __u64 id;
 __u32 pid;
 __u32 flags;
 struct seccomp_data data;
};
struct seccomp_notif_resp {
 __u64 id;
 __s64 val;
 __s32 error;
 __u32 flags;
};
struct rusage {
 struct timeval ru_utime;
 struct timeval ru_stime;
 __kernel_long_t ru_maxrss;
 __kernel_long_t ru_ixrss;
 __kernel_long_t ru_idrss;
 __kernel_long_t ru_isrss;
 __kernel_long_t ru_minflt;
 __kernel_long_t ru_majflt;
 __kernel_long_t ru_nswap;
 __kernel_long_t ru_inblock;
 __kernel_long_t ru_oublock;
 __kernel_long_t ru_msgsnd;
 __kernel_long_t ru_msgrcv;
 __kernel_long_t ru_nsignals;
 __kernel_long_t ru_nvcsw;
 __kernel_long_t ru_nivcsw;
};
struct rlimit {
 __kernel_ulong_t rlim_cur;
 __kernel_ulong_t rlim_max;
};
typedef unsigned long rlim_t;
struct rlimit64 {
 __u64 rlim_cur;
 __u64 rlim_max;
};
struct udiv_t { unsigned int quot, rem; };
extern int errno;
struct tm {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
 long __tm_gmtoff;
 const char *__tm_zone;
};
struct __dirstream{
 off_t tell;
 int fd;
 int buf_pos;
 int buf_end;
 int align;
 char buf[0];
};
typedef struct __dirstream DIR;
struct dirent {
 ino_t d_ino;
 off_t d_off;
 unsigned short d_reclen;
 char d_name[];
};
typedef struct {char c;} __guarded_memory;
typedef unsigned int index_t;
typedef unsigned int brk_data_t;
struct ml_malloc_area{
 void *pos;
 unsigned short int size;
 unsigned short int free;
 unsigned short int freegaps;
 unsigned short int largestfree;
};
struct ml_malloc_def;
struct ml_malloc_freearray{
  unsigned int pos;
  unsigned int largestfree;
  unsigned int data[0];
  unsigned int size;
  unsigned int freegaps;
  unsigned int _data[];
};
struct ml_malloc_def{
 struct ml_malloc_area *area;
 struct ml_malloc_freestorage* free;
 void* (*find_free)(struct ml_malloc_def*,int size);
 int (*resize_area)(struct ml_malloc_area*,int size);
 int (*check_area)(struct ml_malloc_area*);
 const int movposfactor;
 const int typesize;
 const int btval,btfree,btprevfree,bttop;
};
typedef struct ml_malloc_area ml_malloc_area_t;
static void ml_ewriteui(unsigned int);
static void ewriteui(unsigned int i){
 ml_ewriteui(i);
}
static void __attribute__((noreturn))__ERR(int errnum, const char* func );
static inline void __attribute__((noreturn,always_inline))exit(int ret){
 asm("jmp __exit"::"D"(ret));
 __builtin_unreachable();
}
typedef struct {
 int tm_sec;
 int tm_min;
 int tm_hour;
 int tm_mday;
 int tm_mon;
 int tm_year;
 int tm_wday;
 int tm_yday;
 int tm_isdst;
} tm;
typedef struct {
  time_t actime;
  time_t modtime;
} utimbuf;
extern void volatile __attribute__((naked,noipa,cold)) opt_fence(void*p,...);
asm(".global opt_fence\nopt_fence:\nretq");
struct stat;
struct timeval;
struct timezone;
typedef unsigned long u64;
typedef unsigned int u32;
typedef long aio_context_t;
typedef long key_serial_t;
typedef long loff_t;
typedef long qid_t;
typedef long mqd_t;
typedef long stack_t;
typedef long umode_t;
typedef long cap_user_data_t;
typedef long cap_user_header_t;
typedef long semun_u;
struct sockaddr;
struct iovcc;
struct rusage;
struct file_handle;
struct utimbuf;
struct sched_attr;
struct sched_param;
struct itimerspec;
struct kexec_segment;
struct task_struct;
struct msghdr;
struct iocb;
struct io_event;
struct linux_dirent64;
struct pt_regs;
struct perf_event_attr;
struct __sysctl_args;
struct timex;
struct msgbuf;
struct pollfd;
struct getcpu_cache;
struct bpf_attr;
struct shmid_ds;
struct epoll_event;
struct mq_attr;
struct poll_fd;
struct mq_attr;
struct sembuf;
struct statfs;
struct sysinfo;
struct rlimit;
struct ustat;
struct itimerval;
struct linux_dirent;
struct robust_list_head;
struct sigevent;
struct siginfo;
struct bpf_attr;
struct msqid_ds;
struct rlimit64;
struct sigaction;
struct mmsghdr;
struct msqid_ds;

























































;






















































#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Wint-conversion"
static inline int volatile __attribute__((always_inline)) write( int a1, const void *a2, int a3 ){ int sysret; asm volatile ("syscall" : "=a" (sysret) : "a" ( (( 1 | 0 ) | 0 ) ) , "D" (a1) , "S" (a2) , "d" (a3) : "memory","rcx", "r11" ); return( sysret ); }
#pragma GCC diagnostic pop
static int nread(int fd, char *buf, int len);
static int rwfd(const int syscallnumber, int fd, char *buf, int len);
static ml_size_t strlen(const char*str);
static void* memcpy( void*d, const void *s, ml_size_t n );
static void ml_ewriteui(unsigned int ui);
static void* memcpy8( void*d, const void *s, ml_size_t n );
static inline void bzero(void *s, int n);
static inline void bzero8(void *s, int n);
static inline void *memset( void *s, int c, ml_size_t n);
typedef __builtin_va_list __gnuc_va_list;
typedef __gnuc_va_list va_list;
       
void __attribute__((naked,noreturn))_start(){
__asm__ volatile("#.global _start\n#_start:\n xorl %ebp, %ebp\n	popq %rdi\n	movq %rsp,%rsi\n	movq %rdi,%rax\n	leaq  8(%rsi,%rdi,8),%rdx\n"
 "call main\n"
 "movq %rax, %rdi\n"
".global __exit\n"
 "__exit:\n"
 "movq $60, %rax\n"
 "syscall\n"
 "xor %esp,%esp\nret"
 );
};
static void ml_ewriteui(unsigned int ui){
 unsigned int _ewriteui( unsigned int i, int digit){
  if ( digit*10 < i )
   i = _ewriteui(i,digit*10);
  char c = '0' + i/digit;
  i -= i/digit * digit;
  write(1,&c,1);
  return(i);
 }
 _ewriteui(ui,1);
}
static void* memcpy8( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsq" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
static int nread(int fd, char *buf, int len){
 return(rwfd(0 | 0,fd,buf,len));
}
static inline void bzero(void *s, int n){
 asm volatile("xor %%eax,%%eax\n rep stosb\n" : "+c"(n), "+D"(s) :: "eax", "memory" );
}
static inline void bzero8(void *s, int n){
 asm volatile("xor %%eax,%%eax\n rep stosq\n" : "+c"(n), "+D"(s) :: "eax", "memory" );
}
static inline void *memset( void *s, int c, ml_size_t n){
 char *ret = s;
 asm __volatile__( "rep stosb": "+D"(s), "+c"(n) : "a"(c) : "memory","cc" );
 return(ret);
}
static int rwfd(const int syscallnumber, int fd, char *buf, int len){
 char *b = buf;
 char *e = buf+len;
 int ret;
 do {
  asm volatile ("syscall" : "=a" (ret) : "a" ( (syscallnumber | 0 ) ) , "D" (fd), "S" (b), "d" ((e-b)) : "memory","rcx", "r11" );
  if ( ret <= 0 ){
   if ( ret == -11 || ret == -12 || ret == -4 )
    continue;
   return( b-buf ? b-buf : ret );
  }
  b+=ret;
 } while ( b < e );
 return( len );
}
static ml_size_t strlen(const char*str){
 if ( !str )
  return(0);
 unsigned int a;
 asm volatile( R"(
	xor %%ecx, %%ecx
	dec %%ecx
	xor %%eax,%%eax	
	repne scasb	
	neg %%ecx	
	sub $2,%%ecx
 )" : "=c"(a), "+D"(str): : "eax","cc" );
 return (a);
}
static void* memcpy( void*d, const void *s, ml_size_t n ){
 char *ret = d;
 asm volatile("rep movsb" :
  "+c"(n),"+S"(s), "+D"(d) :: "memory","cc" );
 return(ret);
}
typedef struct {
  uint32_t cv[8];
  uint64_t chunk_counter;
  uint8_t buf[64];
  uint8_t buf_len;
  uint8_t blocks_compressed;
  uint8_t flags;
} blake3_chunk_state;
typedef struct {
  uint32_t key[8];
  blake3_chunk_state chunk;
  uint8_t cv_stack_len;
  uint8_t cv_stack[(54 + 1) * 32];
} blake3_hasher;
void blake3_hasher_init(blake3_hasher *self);
void blake3_hasher_init_keyed(blake3_hasher *self,
                              const uint8_t key[32]);
void blake3_hasher_init_derive_key(blake3_hasher *self, const char *context);
void blake3_hasher_update(blake3_hasher *self, const void *input,
                          long input_len);
void blake3_hasher_finalize(const blake3_hasher *self, uint8_t *out,
                            long out_len);
void blake3_hasher_finalize_seek(const blake3_hasher *self, uint64_t seek,
                                 uint8_t *out, long out_len);
enum blake3_flags {
  CHUNK_START = 1 << 0,
  CHUNK_END = 1 << 1,
  PARENT = 1 << 2,
  ROOT = 1 << 3,
  KEYED_HASH = 1 << 4,
  DERIVE_KEY_CONTEXT = 1 << 5,
  DERIVE_KEY_MATERIAL = 1 << 6,
};
static const uint32_t IV[8] = {0x6A09E667UL, 0xBB67AE85UL, 0x3C6EF372UL,
                               0xA54FF53AUL, 0x510E527FUL, 0x9B05688CUL,
                               0x1F83D9ABUL, 0x5BE0CD19UL};
static const uint8_t MSG_SCHEDULE[7][16] = {
    {0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15},
    {2, 6, 3, 10, 7, 0, 4, 13, 1, 11, 12, 5, 9, 14, 15, 8},
    {3, 4, 10, 12, 13, 2, 7, 14, 6, 5, 9, 0, 11, 15, 8, 1},
    {10, 7, 12, 9, 14, 3, 13, 15, 4, 0, 11, 2, 5, 8, 1, 6},
    {12, 13, 9, 11, 15, 10, 14, 8, 7, 2, 5, 3, 0, 1, 6, 4},
    {9, 14, 11, 5, 8, 12, 15, 1, 13, 3, 0, 10, 2, 6, 4, 7},
    {11, 15, 5, 0, 1, 9, 8, 6, 14, 10, 2, 12, 3, 4, 7, 13},
};
static unsigned int highest_one(uint64_t x) {
  return 63 ^ __builtin_clzll(x);
}
static unsigned int popcnt(uint64_t x) {
  unsigned int count = 0;
  while (x != 0) {
    count += 1;
    x &= x - 1;
  }
  return count;
}
static uint64_t round_down_to_power_of_2(uint64_t x) {
  return 1ULL << highest_one(x | 1);
}
static uint32_t counter_low(uint64_t counter) { return (uint32_t)counter; }
static uint32_t counter_high(uint64_t counter) {
  return (uint32_t)(counter >> 32);
}
static uint32_t load32(const void *src) {
  return(*((uint*)src));
}
static void load_key_words(const uint8_t key[32],
                           uint32_t key_words[8]) {
  key_words[0] = load32(&key[0 * 4]);
  key_words[1] = load32(&key[1 * 4]);
  key_words[2] = load32(&key[2 * 4]);
  key_words[3] = load32(&key[3 * 4]);
  key_words[4] = load32(&key[4 * 4]);
  key_words[5] = load32(&key[5 * 4]);
  key_words[6] = load32(&key[6 * 4]);
  key_words[7] = load32(&key[7 * 4]);
}
void blake3_compress_in_place(uint32_t cv[8],
                              const uint8_t block[64],
                              uint8_t block_len, uint64_t counter,
                              uint8_t flags);
void blake3_compress_xof(const uint32_t cv[8],
                         const uint8_t block[64],
                         uint8_t block_len, uint64_t counter, uint8_t flags,
                         uint8_t out[64]);
void blake3_hash_many(const uint8_t *const *inputs, long num_inputs,
                      long blocks, const uint32_t key[8], uint64_t counter,
                      bool increment_counter, uint8_t flags,
                      uint8_t flags_start, uint8_t flags_end, uint8_t *out);
long blake3_simd_degree(void);
void blake3_compress_in_place_portable(uint32_t cv[8],
                                       const uint8_t block[64],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags);
void blake3_compress_xof_portable(const uint32_t cv[8],
                                  const uint8_t block[64],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]);
void blake3_hash_many_portable(const uint8_t *const *inputs, long num_inputs,
                               long blocks, const uint32_t key[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out);
static void chunk_state_init(blake3_chunk_state *self, const uint32_t key[8],
                             uint8_t flags) {
  memcpy(self->cv, key, 32);
  self->chunk_counter = 0;
  memset(self->buf, 0, 64);
  self->buf_len = 0;
  self->blocks_compressed = 0;
  self->flags = flags;
}
static void chunk_state_reset(blake3_chunk_state *self, const uint32_t key[8],
                              uint64_t chunk_counter) {
  memcpy(self->cv, key, 32);
  self->chunk_counter = chunk_counter;
  self->blocks_compressed = 0;
  memset(self->buf, 0, 64);
  self->buf_len = 0;
}
static long chunk_state_len(const blake3_chunk_state *self) {
  return (64 * (long)self->blocks_compressed) +
         ((long)self->buf_len);
}
static long chunk_state_fill_buf(blake3_chunk_state *self,
                                   const uint8_t *input, long input_len) {
  long take = 64 - ((long)self->buf_len);
  if (take > input_len) {
    take = input_len;
  }
  uint8_t *dest = self->buf + ((long)self->buf_len);
  memcpy(dest, input, take);
  self->buf_len += (uint8_t)take;
  return take;
}
static uint8_t chunk_state_maybe_start_flag(const blake3_chunk_state *self) {
  if (self->blocks_compressed == 0) {
    return CHUNK_START;
  } else {
    return 0;
  }
}
typedef struct {
  uint32_t input_cv[8];
  uint64_t counter;
  uint8_t block[64];
  uint8_t block_len;
  uint8_t flags;
} output_t;
static output_t make_output(const uint32_t input_cv[8],
                            const uint8_t block[64],
                            uint8_t block_len, uint64_t counter,
                            uint8_t flags) {
  output_t ret;
  memcpy(ret.input_cv, input_cv, 32);
  memcpy(ret.block, block, 64);
  ret.block_len = block_len;
  ret.counter = counter;
  ret.flags = flags;
  return ret;
}
static void output_chaining_value(const output_t *self, uint8_t cv[32]) {
  uint32_t cv_words[8];
  memcpy(cv_words, self->input_cv, 32);
  blake3_compress_in_place(cv_words, self->block, self->block_len,
                           self->counter, self->flags);
  memcpy(cv, cv_words, 32);
}
static void output_root_bytes(const output_t *self, uint64_t seek, uint8_t *out,
                              long out_len) {
  uint64_t output_block_counter = seek / 64;
  long offset_within_block = seek % 64;
  uint8_t wide_buf[64];
  while (out_len > 0) {
    blake3_compress_xof(self->input_cv, self->block, self->block_len,
                        output_block_counter, self->flags | ROOT, wide_buf);
    long available_bytes = 64 - offset_within_block;
    long memcpy_len;
    if (out_len > available_bytes) {
      memcpy_len = available_bytes;
    } else {
      memcpy_len = out_len;
    }
    memcpy(out, wide_buf + offset_within_block, memcpy_len);
    out += memcpy_len;
    out_len -= memcpy_len;
    output_block_counter += 1;
    offset_within_block = 0;
  }
}
static void chunk_state_update(blake3_chunk_state *self, const uint8_t *input,
                               long input_len) {
  if (self->buf_len > 0) {
    long take = chunk_state_fill_buf(self, input, input_len);
    input += take;
    input_len -= take;
    if (input_len > 0) {
      blake3_compress_in_place(
          self->cv, self->buf, 64, self->chunk_counter,
          self->flags | chunk_state_maybe_start_flag(self));
      self->blocks_compressed += 1;
      self->buf_len = 0;
      memset(self->buf, 0, 64);
    }
  }
  while (input_len > 64) {
    blake3_compress_in_place(self->cv, input, 64,
                             self->chunk_counter,
                             self->flags | chunk_state_maybe_start_flag(self));
    self->blocks_compressed += 1;
    input += 64;
    input_len -= 64;
  }
  long take = chunk_state_fill_buf(self, input, input_len);
  input += take;
  input_len -= take;
}
static output_t chunk_state_output(const blake3_chunk_state *self) {
  uint8_t block_flags =
      self->flags | chunk_state_maybe_start_flag(self) | CHUNK_END;
  return make_output(self->cv, self->buf, self->buf_len, self->chunk_counter,
                     block_flags);
}
static output_t parent_output(const uint8_t block[64],
                              const uint32_t key[8], uint8_t flags) {
  return make_output(key, block, 64, 0, flags | PARENT);
}
static long left_len(long content_len) {
  long full_chunks = (content_len - 1) / 1024;
  return round_down_to_power_of_2(full_chunks) * 1024;
}
static long compress_chunks_parallel(const uint8_t *input, long input_len,
                                       const uint32_t key[8],
                                       uint64_t chunk_counter, uint8_t flags,
                                       uint8_t *out) {
  const uint8_t *chunks_array[16];
  long input_position = 0;
  long chunks_array_len = 0;
  while (input_len - input_position >= 1024) {
    chunks_array[chunks_array_len] = &input[input_position];
    input_position += 1024;
    chunks_array_len += 1;
  }
  blake3_hash_many(chunks_array, chunks_array_len,
                   1024 / 64, key, chunk_counter,
                   1, flags, CHUNK_START, CHUNK_END, out);
  if (input_len > input_position) {
    uint64_t counter = chunk_counter + (uint64_t)chunks_array_len;
    blake3_chunk_state chunk_state;
    chunk_state_init(&chunk_state, key, flags);
    chunk_state.chunk_counter = counter;
    chunk_state_update(&chunk_state, &input[input_position],
                       input_len - input_position);
    output_t output = chunk_state_output(&chunk_state);
    output_chaining_value(&output, &out[chunks_array_len * 32]);
    return chunks_array_len + 1;
  } else {
    return chunks_array_len;
  }
}
static long compress_parents_parallel(const uint8_t *child_chaining_values,
                                        long num_chaining_values,
                                        const uint32_t key[8], uint8_t flags,
                                        uint8_t *out) {
  const uint8_t *parents_array[(16 > 2 ? 16 : 2)];
  long parents_array_len = 0;
  while (num_chaining_values - (2 * parents_array_len) >= 2) {
    parents_array[parents_array_len] =
        &child_chaining_values[2 * parents_array_len * 32];
    parents_array_len += 1;
  }
  blake3_hash_many(parents_array, parents_array_len, 1, key,
                   0,
                   0, flags | PARENT,
                   0,
                   0,
                   out);
  if (num_chaining_values > 2 * parents_array_len) {
    memcpy(&out[parents_array_len * 32],
           &child_chaining_values[2 * parents_array_len * 32],
           32);
    return parents_array_len + 1;
  } else {
    return parents_array_len;
  }
}
static long blake3_compress_subtree_wide(const uint8_t *input,
                                           long input_len,
                                           const uint32_t key[8],
                                           uint64_t chunk_counter,
                                           uint8_t flags, uint8_t *out) {
  if (input_len <= blake3_simd_degree() * 1024) {
    return compress_chunks_parallel(input, input_len, key, chunk_counter, flags,
                                    out);
  }
  long left_input_len = left_len(input_len);
  long right_input_len = input_len - left_input_len;
  const uint8_t *right_input = &input[left_input_len];
  uint64_t right_chunk_counter =
      chunk_counter + (uint64_t)(left_input_len / 1024);
  uint8_t cv_array[2 * (16 > 2 ? 16 : 2) * 32];
  long degree = blake3_simd_degree();
  if (left_input_len > 1024 && degree == 1) {
    degree = 2;
  }
  uint8_t *right_cvs = &cv_array[degree * 32];
  long left_n = blake3_compress_subtree_wide(input, left_input_len, key,
                                               chunk_counter, flags, cv_array);
  long right_n = blake3_compress_subtree_wide(
      right_input, right_input_len, key, right_chunk_counter, flags, right_cvs);
  if (left_n == 1) {
    memcpy(out, cv_array, 2 * 32);
    return 2;
  }
  long num_chaining_values = left_n + right_n;
  return compress_parents_parallel(cv_array, num_chaining_values, key, flags,
                                   out);
}
static void compress_subtree_to_parent_node(
    const uint8_t *input, long input_len, const uint32_t key[8],
    uint64_t chunk_counter, uint8_t flags, uint8_t out[2 * 32]) {
  uint8_t cv_array[2 * (16 > 2 ? 16 : 2) * 32];
  long num_cvs = blake3_compress_subtree_wide(input, input_len, key,
                                                chunk_counter, flags, cv_array);
  uint8_t out_array[(16 > 2 ? 16 : 2) * 32 / 2];
  while (num_cvs > 2) {
    num_cvs =
        compress_parents_parallel(cv_array, num_cvs, key, flags, out_array);
    memcpy(cv_array, out_array, num_cvs * 32);
  }
  memcpy(out, cv_array, 2 * 32);
}
static void hasher_init_base(blake3_hasher *self, const uint32_t key[8],
                             uint8_t flags) {
  memcpy(self->key, key, 32);
  chunk_state_init(&self->chunk, key, flags);
  self->cv_stack_len = 0;
}
void blake3_hasher_init(blake3_hasher *self) { hasher_init_base(self, IV, 0); }
void blake3_hasher_init_keyed(blake3_hasher *self,
                              const uint8_t key[32]) {
  uint32_t key_words[8];
  load_key_words(key, key_words);
  hasher_init_base(self, key_words, KEYED_HASH);
}
void blake3_hasher_init_derive_key(blake3_hasher *self, const char *context) {
  blake3_hasher context_hasher;
  hasher_init_base(&context_hasher, IV, DERIVE_KEY_CONTEXT);
  blake3_hasher_update(&context_hasher, context, strlen(context));
  uint8_t context_key[32];
  blake3_hasher_finalize(&context_hasher, context_key, 32);
  uint32_t context_key_words[8];
  load_key_words(context_key, context_key_words);
  hasher_init_base(self, context_key_words, DERIVE_KEY_MATERIAL);
}
static void hasher_merge_cv_stack(blake3_hasher *self, uint64_t total_len) {
  long post_merge_stack_len = (long)popcnt(total_len);
  while (self->cv_stack_len > post_merge_stack_len) {
    uint8_t *parent_node =
        &self->cv_stack[(self->cv_stack_len - 2) * 32];
    output_t output = parent_output(parent_node, self->key, self->chunk.flags);
    output_chaining_value(&output, parent_node);
    self->cv_stack_len -= 1;
  }
}
static void hasher_push_cv(blake3_hasher *self, uint8_t new_cv[32],
                           uint64_t chunk_counter) {
  hasher_merge_cv_stack(self, chunk_counter);
  memcpy(&self->cv_stack[self->cv_stack_len * 32], new_cv,
         32);
  self->cv_stack_len += 1;
}
void blake3_hasher_update(blake3_hasher *self, const void *input,
                          long input_len) {
  if (input_len == 0) {
    return;
  }
  const uint8_t *input_bytes = (const uint8_t *)input;
  if (chunk_state_len(&self->chunk) > 0) {
    long take = 1024 - chunk_state_len(&self->chunk);
    if (take > input_len) {
      take = input_len;
    }
    chunk_state_update(&self->chunk, input_bytes, take);
    input_bytes += take;
    input_len -= take;
    if (input_len > 0) {
      output_t output = chunk_state_output(&self->chunk);
      uint8_t chunk_cv[32];
      output_chaining_value(&output, chunk_cv);
      hasher_push_cv(self, chunk_cv, self->chunk.chunk_counter);
      chunk_state_reset(&self->chunk, self->key, self->chunk.chunk_counter + 1);
    } else {
      return;
    }
  }
  while (input_len > 1024) {
    long subtree_len = round_down_to_power_of_2(input_len);
    uint64_t count_so_far = self->chunk.chunk_counter * 1024;
    while ((((uint64_t)(subtree_len - 1)) & count_so_far) != 0) {
      subtree_len /= 2;
    }
    uint64_t subtree_chunks = subtree_len / 1024;
    if (subtree_len <= 1024) {
      blake3_chunk_state chunk_state;
      chunk_state_init(&chunk_state, self->key, self->chunk.flags);
      chunk_state.chunk_counter = self->chunk.chunk_counter;
      chunk_state_update(&chunk_state, input_bytes, subtree_len);
      output_t output = chunk_state_output(&chunk_state);
      uint8_t cv[32];
      output_chaining_value(&output, cv);
      hasher_push_cv(self, cv, chunk_state.chunk_counter);
    } else {
      uint8_t cv_pair[2 * 32];
      compress_subtree_to_parent_node(input_bytes, subtree_len, self->key,
                                      self->chunk.chunk_counter,
                                      self->chunk.flags, cv_pair);
      hasher_push_cv(self, cv_pair, self->chunk.chunk_counter);
      hasher_push_cv(self, &cv_pair[32],
                     self->chunk.chunk_counter + (subtree_chunks / 2));
    }
    self->chunk.chunk_counter += subtree_chunks;
    input_bytes += subtree_len;
    input_len -= subtree_len;
  }
  if (input_len > 0) {
    chunk_state_update(&self->chunk, input_bytes, input_len);
    hasher_merge_cv_stack(self, self->chunk.chunk_counter);
  }
}
void blake3_hasher_finalize(const blake3_hasher *self, uint8_t *out,
                            long out_len) {
  blake3_hasher_finalize_seek(self, 0, out, out_len);
}
void blake3_hasher_finalize_seek(const blake3_hasher *self, uint64_t seek,
                                 uint8_t *out, long out_len) {
  if (out_len == 0) {
    return;
  }
  if (self->cv_stack_len == 0) {
    output_t output = chunk_state_output(&self->chunk);
    output_root_bytes(&output, seek, out, out_len);
    return;
  }
  output_t output;
  long cvs_remaining;
  if (chunk_state_len(&self->chunk) > 0) {
    cvs_remaining = self->cv_stack_len;
    output = chunk_state_output(&self->chunk);
  } else {
    cvs_remaining = self->cv_stack_len - 2;
    output = parent_output(&self->cv_stack[cvs_remaining * 32], self->key,
                           self->chunk.flags);
  }
  while (cvs_remaining > 0) {
    cvs_remaining -= 1;
    uint8_t parent_block[64];
    memcpy(parent_block, &self->cv_stack[cvs_remaining * 32], 32);
    output_chaining_value(&output, &parent_block[32]);
    output = parent_output(parent_block, self->key, self->chunk.flags);
  }
  output_root_bytes(&output, seek, out, out_len);
}
static void store32(void *dst, uint32_t w) {
 *((uint*)dst) = w;
}
static void g(uint32_t *state, long a, long b, long c, long d,
              uint32_t x, uint32_t y) {
  state[a] = state[a] + state[b] + x;
  state[d] = ({ uint i = state[d] ^ state[a]; asm("ror $""16"",%0" : "+r"(i) ); i; });
  state[c] = state[c] + state[d];
  state[b] = ({ uint i = state[b] ^ state[c]; asm("ror $""12"",%0" : "+r"(i) ); i; });
  state[a] = state[a] + state[b] + y;
  state[d] = ({ uint i = state[d] ^ state[a]; asm("ror $""8"",%0" : "+r"(i) ); i; });
  state[c] = state[c] + state[d];
  state[b] = ({ uint i = state[b] ^ state[c]; asm("ror $""7"",%0" : "+r"(i) ); i; });
}
static void round_fn(uint32_t state[16], const uint32_t *msg, long round) {
  const uint8_t *schedule = MSG_SCHEDULE[round];
  for ( int a = 0; a<4; a++ )
   g(state, a, a+4, a+8, a+12, msg[schedule[a*2]], msg[schedule[a*2+1]]);
  static const uchar mix[] = { 0,5,10,15,1,6,11,12,2,7,8,13,3,4,9,14 };
  for ( int a = 0; a<4; a++ )
   g(state, mix[a*4], mix[a*4+1], mix[a*4+2], mix[a*4+3],msg[schedule[a*2+8]], msg[schedule[a*2+9]]);
}
static void compress_pre(uint32_t state[16], const uint32_t cv[8],
                         const uint8_t block[64],
                         uint8_t block_len, uint64_t counter, uint8_t flags) {
  uint32_t block_words[16];
  for ( int a = 0; a<8; a++ ){
   *((ulong*)block_words + a) = *((ulong*)block + a);
    state[a] = cv[a];
  }
  state[8] = IV[0];
  state[9] = IV[1];
  state[10] = IV[2];
  state[11] = IV[3];
  state[12] = counter_low(counter);
  state[13] = counter_high(counter);
  state[14] = (uint32_t)block_len;
  state[15] = (uint32_t)flags;
  for ( int a = 0; a<7; a++ )
   round_fn(state, &block_words[0], a);
}
void blake3_compress_in_place(uint32_t cv[8],
                                       const uint8_t block[64],
                                       uint8_t block_len, uint64_t counter,
                                       uint8_t flags) {
  uint32_t state[16];
  compress_pre(state, cv, block, block_len, counter, flags);
  for ( int a = 0; a<4; a++ )
    *((ulong*)cv+a) = *((ulong*)state+a) ^ *((ulong*)state+a+4);
}
void blake3_compress_xof(const uint32_t cv[8],
                                  const uint8_t block[64],
                                  uint8_t block_len, uint64_t counter,
                                  uint8_t flags, uint8_t out[64]) {
  uint32_t state[16];
  compress_pre(state, cv, block, block_len, counter, flags);
  for ( int a = 0; a<8; a++ ){
   store32(&out[a*4], state[a] ^ state[a+8] );
   store32(&out[(a+8)*4], state[a] ^ cv[a] );
  }
}
static void hash_one(const uint8_t *input, long blocks,
                              const uint32_t key[8], uint64_t counter,
                              uint8_t flags, uint8_t flags_start,
                              uint8_t flags_end, uint8_t out[32]) {
  uint32_t cv[8];
  memcpy(cv, key, 32);
  uint8_t block_flags = flags | flags_start;
  while (blocks > 0) {
    if (blocks == 1) {
      block_flags |= flags_end;
    }
    blake3_compress_in_place(cv, input, 64, counter,
                                      block_flags);
    input = &input[64];
    blocks -= 1;
    block_flags = flags;
  }
  memcpy(out, cv, 32);
}
void blake3_hash_many(const uint8_t *const *inputs, long num_inputs,
                               long blocks, const uint32_t key[8],
                               uint64_t counter, bool increment_counter,
                               uint8_t flags, uint8_t flags_start,
                               uint8_t flags_end, uint8_t *out) {
  while (num_inputs > 0) {
    hash_one(inputs[0], blocks, key, counter, flags, flags_start,
                      flags_end, out);
    if (increment_counter) {
      counter += 1;
    }
    inputs += 1;
    num_inputs -= 1;
    out = &out[32];
  }
}
long blake3_simd_degree(void) {
 return 1;
}
int main(int argc, char *argv[]){
 uchar buf[4000];
 uchar obuf[64];
 blake3_hasher ctx;
 int r;
 int blen = 32;
 blake3_hasher_init(&ctx);
 while ( (r=nread(0,(char*)buf,4000)) ){
  blake3_hasher_update(&ctx,buf,r);
 }
 blake3_hasher_finalize(&ctx,obuf,blen);
 for ( int a = 0; a<blen; a++ ){
  char c = obuf[a];
  buf[2*a +1] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
  c >>=4;
  buf[2*a] = (c&0xf)>9 ? ((c&0xf) + 'a' - 0xa) : (c&0xf) + '0';
 }
 buf[blen*2] = '\n';
 write(1,buf,(blen*2+1));
 exit(0);
}
