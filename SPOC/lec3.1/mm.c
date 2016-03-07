#include <unistd.h>


#define size_t unsigned int
#define ALIGN 4096


struct pp
{
    struct pp *next;
    struct pp *prev;
    size_t size;
    int flag;
};

typedef struct pp Header;

Header* base;

// return the header pointer with enough size
Header *morecore(size_t size) {
    char *p;
    Header *header;

    size_t nsize = size + sizeof(Header);
    nsize += (nsize - 1) / ALIGN + 1;

    p = sbrk(nsize * ALIGN);
    if (p == (char *) -1)
        return 0;
    header = (Header *) p;
    header->size = nsize * ALIGN - sizeof(Header);
    header->next = 0;
    header->prev = 0;
    header->flag = 0;
    return header;
}

Header *split(Header *header, size_t size) {
    Header *new;
    if (header->size > size + sizeof(Header)) {
        new = header + sizeof(Header) + size;

        new->prev = header;
        new->size = header->size - size - sizeof(Header);
        new->next = header->next;
        new->flag = 0;

        header->next = new;
        header->size = size;
        return new;
    } else {
        return 0;
    }
}

void *mmalloc(size_t size) {
    Header *header;
    Header *tailer;

    int found;
    if (!base) {
        base = morecore(size);
        base->flag |= 1;
        return base + sizeof(Header);
    } else {
        found = 0;
        for (header = base; header; tailer = header, header = header->next)
            if (~(header->flag & 1) && (header->size >= size)) {
                found = 1;
                break;
            }

        if (found) {
            split(header, size);
            header->flag |= 1;
            return header + sizeof(Header);
        } else {
            header = morecore(size);
            if (header != 0) {
                header->prev = tailer;
                tailer->next = header;
            }
            header->flag |= 1;
            return header + sizeof(Header);
        }
    }
}

Header * merge(Header *h1, Header *h2) {
    h1->next = h2->next;
    h2->next->prev = h1;
    h1->size += h2->size + sizeof(Header);
    return h1;
}

void mfree(void *p) {
    Header *header = (Header *) (p - sizeof(Header));
    header->flag &= 0;
    Header *prev = header->prev;
    Header *next = header->next;
    if (prev && ~(prev->flag & 1)) {
        header = merge(prev, header);
    }

    if (next && ~(next->flag & 1)) {
        header = merge(header, next);
    }
}

int main() {
    int *a = (int *) mmalloc(100);
    int *b = (char *) mmalloc(200);
    mfree(a);
    mfree(b);
    return 0;
}