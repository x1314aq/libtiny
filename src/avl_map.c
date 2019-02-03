//
// Created by x1314aq on 2019/1/29.
//

#include "avl_map.h"


#define MAP_INIT_SIZE    16

int avl_map_init(struct map *m, struct map_type *type)
{
    m->size = MAP_INIT_SIZE;
    m->mask = m->size - 1;
    m->used = 0;
    m->type.hash_func = type->hash_func;
    m->type.key_compare = type->key_compare;
    list_head_init(&m->head);
    mempool_init(&m->mp);
    m->buckets = (struct map_bucket *) malloc(m->size * sizeof(struct map_bucket));
    if(!m->buckets)
        return -1;

    for(uint64_t i = 0; i < m->size; i++) {
        m->buckets[i].root.node = NULL;
        list_head_init(&(m->buckets[i].entry));
    }
    return 0;
}

void avl_map_destroy(struct map *m)
{
    mempool_destroy(&m->mp);
    free(m->buckets);
}

struct map_entry *avl_map_first(struct map *m)
{
    struct list_entry *ptr = m->head.next;

    if(ptr == &m->head)
        return NULL;

    struct map_bucket *bucket = container_of(ptr, struct map_bucket, entry);
    struct avl_node *node = avl_first(&bucket->root);
    struct map_entry *entry = container_of(node, struct map_entry, node);
    return entry;
}

struct map_entry *avl_map_last(struct map *m)
{
    struct list_entry *ptr = m->head.prev;

    if(ptr == &m->head)
        return NULL;

    struct map_bucket *bucket = container_of(ptr, struct map_bucket, entry);
    struct avl_node *node = avl_last(&bucket->root);
    struct map_entry *entry = container_of(node, struct map_entry, node);
    return entry;
}

struct map_entry *avl_map_next(struct map *m, struct map_entry *e)
{
    struct map_entry *entry = NULL;
    struct avl_node *node = avl_next(&e->node);

    if(node) {
        entry = container_of(node, struct map_entry, node);
        return entry;
    }

    uint64_t index = e->hash & m->mask;
    struct map_bucket *bucket = &(m->buckets[index]);
    struct list_entry *list_entry = bucket->entry.next;

    if(list_entry == &m->head)
        return NULL;

    struct map_bucket *next_bucket = container_of(list_entry, struct map_bucket, entry);
    node = avl_first(&next_bucket->root);
    entry = container_of(node, struct map_entry, node);

    return entry;
}

struct map_entry *avl_map_prev(struct map *m, struct map_entry *e)
{
    struct map_entry *entry = NULL;
    struct avl_node *node = avl_prev(&e->node);

    if(node) {
        entry = container_of(node, struct map_entry, node);
        return entry;
    }

    uint64_t index = e->hash & m->mask;
    struct map_bucket *bucket = &(m->buckets[index]);
    struct list_entry *list_entry = bucket->entry.prev;

    if(list_entry == &m->head)
        return NULL;

    struct map_bucket *prev_bucket = container_of(list_entry, struct map_bucket, entry);
    node = avl_last(&prev_bucket->root);
    entry = container_of(node, struct map_entry, node);

    return entry;
}

struct map_entry *avl_map_find(struct map *m, const void *k)
{
    uint64_t hash = m->type.hash_func(k);
    struct map_bucket *bucket = &m->buckets[hash & m->mask];
    struct avl_node *node = bucket->root.node;
    struct map_entry *e;

    while(node) {
        e = container_of(node, struct map_entry, node);
        if(e->hash == hash) {
            int res = m->type.key_compare(k, e->key);
            if(res == 0)
                return e;
            node = res < 0 ? node->left : node->right;
        }
        else
            node = hash < e->hash ? node->left : node->right;
    }
    return NULL;
}

int avl_map_insert(struct map *m, void *k, void *v)
{
    uint64_t hash = m->type.hash_func(k);
    struct map_bucket *bucket = &m->buckets[hash & m->mask];
    struct map_entry *e = mempool_alloc(&m->mp, sizeof(struct map_entry));
    struct map_entry *entry;

    e->hash = hash;
    e->key = k;
    e->val = v;

    if(bucket->root.node) {
        struct avl_node **new, *parent;

        new = &bucket->root.node;
        parent = NULL;
        while(*new) {
            parent = *new;
            entry = container_of(parent, struct map_entry, node);
            if(entry->hash == hash) {
                int res = m->type.key_compare(k, entry->key);
                if(res == 0) {   /// found duplicate entry
                    mempool_dealloc(&m->mp, e, sizeof(struct map_entry));
                    return -1;
                }
                new = res < 0 ? &parent->left : &parent->right;
            }
            else
                new = hash < entry->hash ? &parent->left : &parent->right;
        }
        avl_link_node(&e->node, parent, new);
        avl_insert_rotate(&e->node, &bucket->root);
    }
    else {
        bucket->root.node = &e->node;
        e->node.left = NULL;
        e->node.right = NULL;
        e->node.parent = NULL;
        e->node.height = 1;
        list_add_before(&bucket->entry, &m->head);
    }
    m->used++;

    return 0;
}

void avl_map_erase(struct map *m, struct map_entry *e)
{
    uint64_t hash = e->hash;
    struct map_bucket *bucket = &m->buckets[hash & m->mask];

    if(bucket->root.node == &e->node) {
        bucket->root.node = NULL;
        list_delete(&bucket->entry);
    }
    else
        avl_erase(&e->node, &bucket->root);
    mempool_dealloc(&m->mp, e, sizeof(struct map_entry));
    m->used--;
}
