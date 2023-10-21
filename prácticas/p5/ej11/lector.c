#define REG_LECTOR 0

int driver_read(char* udata, int size) {
    assert(size == sizeof(int));
    int codigo = IN(REG_LECTOR);
    copy_to_user(udata, &codigo, size);
    return size;
}
