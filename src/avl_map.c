//
// Created by x1314aq on 2019/1/29.
//

#include "avl_map.h"


#define MAP_INIT_SIZE    16

void avl_map_init(struct map *m, struct map_type *type)
{
    m->size = MAP_INIT_SIZE;
    m->mask = m->size - 1;
    m->used = 0;
    m->type = type;
    list_head_init(&m->head);
    m->buckets = (struct map_bucket *) malloc(sizeof(struct map_bucket) * MAP_INIT_SIZE);

    for(uint64_t i = 0; i < m->size; i++) {
        m->buckets[i].root.node = NULL;
        list_head_init(&(m->buckets[i].entry));
    }
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
    uint64_t hash = m->type->hash_func(k);
    struct map_bucket *bucket = &m->buckets[hash & m->mask];
    struct avl_node *node = bucket->root.node;
    struct map_entry *e;

    while(node) {
        e = container_of(node, struct map_entry, node);
        if(e->hash == hash) {
            int res = m->type->key_compare(k, e->key);
            if(res == 0)
                return e;
            node = res < 0 ? node->left : node->right;
        }
        else
            node = hash < e->hash ? node->left : node->right;
    }
    return NULL;
}

int avl_map_insert(struct map *m, struct map_entry *e)
{
    uint64_t hash = m->type->hash_func(e->key);
    struct map_bucket *bucket = &m->buckets[hash & m->mask];
    struct map_entry *entry;

    e->hash = hash;

    if(bucket->root.node) {
        struct avl_node **new, *parent;

        new = &bucket->root.node;
        parent = NULL;
        while(*new) {
            parent = *new;
            entry = container_of(parent, struct map_entry, node);
            if(entry->hash == hash) {
                int res = m->type->key_compare(e->key, entry->key);
                if(res == 0)    /// found duplicate entry
                    return -1;
                new = res < 0 ? &parent->left : &parent->right;
            }
            else
                new = hash < entry->hash ? &parent->left : &parent->right;
        }
        avl_link_node(&e->node, parent, new);
        avl_insert_rotate(&e->node, &bucket->root);
    }
    else {
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
    e->node.left = NULL;
    e->node.right = NULL;
    e->node.parent = NULL;
    m->used--;
}
