//
//  Created by x1314aq on 2017/7/26.
//

#include "algorithm.h"
#include "heap.h"


#define UINT_MAX  0xFFFFFFFF


int *build_bc(const char *p){
    int *bc = malloc(256 * sizeof(int));
    for(size_t i = 0; i < 256; i++){
        bc[i] = -1;
    }
    size_t m = strlen(p);
    for(int j = 0; j < m; j++){
        bc[p[j]] = j;
    }
    return bc;
}

int *build_ss(const char *p){
    int m = (int)strlen(p);
    int *ss = malloc(m * sizeof(int));
    ss[m - 1] = m;
    for(int low = m- 1, high = m - 1, j = low - 1; j >= 0; j--){
        if((low < j) && (ss[m - high + j - 1] <= j - low)) ss[j] = ss[m - high + j - 1];
        else{
            high = j;
            low = MIN(low, high);
            while((low >= 0) && (p[low] == p[m - high + low - 1])) low--;
            ss[j] = high - low;
        }
    }
    return ss;
}

int *build_gs(const char *p){
    int *ss = build_ss(p);
    size_t m = strlen(p);
    int *gs = malloc(m * sizeof(int));
    for(size_t j = 0; j < m; j++) gs[j] = (int)m;
    for(size_t i = 0, j = m - 1; j < UINT_MAX; j--){
        if(j + 1 == ss[j]){
            while(i < m - j - 1) gs[i++] = (int)(m - j - 1);
        }
    }
    for(size_t j = 0; j < m - 1; j++){
        gs[m - ss[j] - 1] = (int)(m - j - 1);
    }
    free(ss);
    return gs;
}

int *build_next(const char *p){
    size_t m = strlen(p), j = 0;
    int *next = malloc(m * sizeof(int));
    next[0] = -1;
    int t = -1;
    while(j < m){
        if(t < 0 || p[j] == p[t]){
            j++;
            t++;
            next[j] = (p[j] != p[t] ? t : next[t]);
        }
        else t = next[t];
    }
    return next;
}

/*************************************************/

int bf_match(const char *t, const char *p){
    size_t n = strlen(t), i = 0;
    size_t m = strlen(p), j = 0;
    while (i < n && j < m) {
        if(t[i] == p[j]){
            i++;
            j++;
        }
        else{
            i = i - j + 1;
            j = 0;
        }
    }
    return i - j;
}

int kmp_match(const char *t, const char *p){
    int *next = build_next(p);
    int n = (int)strlen(t), i = 0;
    int m = (int)strlen(p), j = 0;
    while(i < n && j < m){
        if(j < 0 || t[i] == p[j]){
            i++;
            j++;
        }
        else j = next[j];
    }
    free(next);
    return i - j;
}

int bm_match(const char *t, const char *p){
    int *bc = build_bc(p);
    int *gs = build_gs(p);
    size_t i = 0;
    while (strlen(t) >= i + strlen(p)) {
        int j = (int)strlen(p) - 1;
        while(p[j] == t[i + j])
            if(0 > --j) break;
        if(0 > j) break;
        else i += MAX(gs[j], j - bc[t[i + j]]);
    }
    free(bc);
    free(gs);
    return i;
}

/* 逐字节的交换两个元素的值 */
#define SWAP(base, a, b, size)               \
    do{                                      \
        size_t __size = (size);              \
        char *__a = (base) + __size * (a);   \
        char *__b = (base) + __size * (b);   \
        do{                                  \
            char __tmp = *__a;               \
            *__a++ = *__b;                   \
            *__b++ = __tmp;                  \
        } while(--__size > 0);               \
    } while(0)

#ifdef OLD_QSORT
void quick_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    if(high - low < 2) return;
    SWAP(base, low, (low + high) / 2, ele_size);
    int mid = low;
    for(int i = low + 1; i < high; i++){
        if((*cmp)(base + ele_size * i, base + ele_size * low) < 0){
            mid++;
            SWAP(base, i, mid, ele_size);
        }
    }
    SWAP(base, low, mid, ele_size);
    quick_sort(base, low, mid, ele_size, cmp);
    quick_sort(base, mid + 1, high, ele_size, cmp);
}

#else
/* 使用insertion_sort的截断条件 */
#define INSERTION_SORT_CUTOFF   8

/* 使用median-of-3的截断条件 */
#define MEDIAN_OF_3_CUTOFF      40

int median3(void *base, int low, int mid, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    if((*cmp)(base + low * ele_size, base + mid * ele_size) < 0){
        if((*cmp)(base + mid * ele_size, base + high * ele_size) < 0) return mid;
        else if((*cmp)(base + low * ele_size, base + high * ele_size) < 0) return high;
        else return low;
    }
    else{
        if((*cmp)(base + high * ele_size, base + mid * ele_size) < 0) return mid;
        else if((*cmp)(base + low * ele_size, base + high * ele_size) < 0) return low;
        else return high;
    }
}

void quick_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    int length = high - low;
    if(length <= INSERTION_SORT_CUTOFF){
        insertion_sort(base, low, high, ele_size, cmp);
        return;
    }
    else if(length <= MEDIAN_OF_3_CUTOFF){
        int mid = low + ((high - low) >> 1);
        int median = median3(base, low, mid, high, ele_size, cmp);
        SWAP(base, low, median, ele_size);
    }
    else{
        int eps = length / 8;
        int mid = low + ((high - low) >> 1);
        int first = median3(base, low, low + eps, low + eps + eps, ele_size, cmp);
        int second = median3(base, mid - eps, mid, mid + eps, ele_size, cmp);
        int third = median3(base, high - eps - eps, high - eps, high, ele_size, cmp);
        int ninther = median3(base, first, second, third, ele_size, cmp);
        SWAP(base, low, ninther, ele_size);
    }

    int i = low, j = high;
    int p = low, q = high;
    while(1){
        while((*cmp)(base + (++i) * ele_size, base + low * ele_size) < 0)
            if(i == (high - 1)) break;
        while((*cmp)(base + low * ele_size, base + (--j) * ele_size) < 0)
            if(j == low) break;

        if(i == j && ((*cmp)(base + low * ele_size, base + i * ele_size) == 0)){
            p++;
            SWAP(base, i, p, ele_size);
        }
        if(i >= j) break;

        SWAP(base, i, j, ele_size);
        if((*cmp)(base + low * ele_size, base + i * ele_size) == 0){
            p++;
            SWAP(base, p, i, ele_size);
        }
        if((*cmp)(base + low * ele_size, base + j * ele_size) == 0){
            q--;
            SWAP(base, q, j, ele_size);
        }
    }
    
    i = j + 1;
    for(int k = low; k <= p; k++){
        SWAP(base, k, j, ele_size);
        j--;
    }
    for(int k = high - 1; k >= q; k--){
        SWAP(base, k, i, ele_size);
        i++;
    }

    quick_sort(base, low, j + 1, ele_size, cmp);
    quick_sort(base, i, high, ele_size, cmp);
}
#endif

void selection_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    if(high - low < 2) return;
    while(high > low){
        int p, q;
        for(p = low, q = low + 1; q < high; q++){
            if((*cmp)(base + ele_size * p, base + ele_size * q) < 0) p = q;
        }
        SWAP(base, p, --high, ele_size);
    }
}

void insertion_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)) {
    if(high - low < 2) return;
    int i, j;
    void *val = malloc(ele_size);
    for(i = low + 1; i < high; i++){
        memcpy(val, base + i * ele_size, ele_size);
        for(j = i - 1; j >= 0 && cmp(val, base + ele_size * j) < 0; j--)
            SWAP(base, j + 1, j, ele_size);
        memcpy(base + (j + 1) * ele_size, val, ele_size);
    }
    free(val);
}

void heap_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    if(high - low < 2) return;
    heap *hp = heap_heapify(base, high - low, ele_size, cmp);
    while(--high > low){
        heap_rm_max(hp);
    }
    free(hp);
}

#define MEMCPY(src, des, size)           \
    do{                                  \
        size_t __size = (size);          \
        register char *__des = (des);    \
        register char *__src = (src);    \
        do{                              \
            *__des++ = *__src++;         \
        } while(--__size > 0);           \
    } while(0)

void merge_sort(void *base, int low, int high, size_t ele_size, int (*cmp)(const void *, const void *)){
    if(high - low < 2) return;
    void *back = base;
    void *buf = malloc(ele_size * (high - low));
    int remainder = 0;
    for(int width = 1; width < high - low; width = 2 * width){
        for(int start = 0; start < high - low; start += 2 * width){
            int lo = start, mid = MIN(start + width, high - low), hi = MIN(start + 2 * width, high - low);
            if(mid == (high - low)) remainder = start;
            int k = lo;
            int start1 = lo, end1 = mid;
            int start2 = mid, end2 = hi;
            while(start1 < end1 && start2 < end2){
                if((*cmp)(base + ele_size * start1, base + ele_size * start2) < 0){
                    MEMCPY(base + start1 * ele_size, buf + k * ele_size, ele_size);
                    start1++; 
                }
                else{
                    MEMCPY(base + start2 * ele_size, buf + k * ele_size, ele_size);
                    start2++;
                }
                k++;
            }
            if(start1 < end1) MEMCPY(base + start1 * ele_size, buf + k * ele_size, (end1 - start1) * ele_size);
            if(start2 < end2) MEMCPY(base + start2 * ele_size, buf + k * ele_size, (end2 - start2) * ele_size);
        }
        void *temp = base;
        base = buf;
        buf = temp;
    }
    if(remainder > 0){
        int start1 = 0, end1 = remainder;
        int start2 = remainder, end2 = high;
        int k = 0;
        while(start1 < end1 && start2 < end2){
            if((*cmp)(base + ele_size * start1, base + ele_size * start2) < 0){
                MEMCPY(base + start1 * ele_size, buf + k * ele_size, ele_size);
                start1++;
            }
            else{
                MEMCPY(base + start2 * ele_size, buf + k * ele_size, ele_size);
                start2++;
            }
            k++;
        }
        if(start1 < end1) MEMCPY(base + start1 * ele_size, buf + k * ele_size, (end1 - start1) * ele_size); 
        if(start2 < end2) MEMCPY(base + start2 * ele_size, buf + k * ele_size, (end2 - start2) * ele_size);
        void *temp = base;
        base = buf;
        buf = temp;
    }
    if(base != back){
        MEMCPY(buf, base, (high - low) * ele_size);
        buf = base;
    }
    free(buf);
}


