//
// Find total size of the ELF executable
//
#if 0
static unsigned find_elf32_size(const Elf32_Ehdr *hdr, const Elf32_Phdr segment[])
{
    unsigned vaddr_min = 0, vaddr_max = 0;

    for (int i = 0; i < hdr->e_phnum; i++) {
        // Chooze loadable non-empty segments.
        if (segment[i].p_type == PT_LOAD && segment[i].p_memsz > 0) {
            unsigned first = segment[i].p_vaddr;
            unsigned last  = first + segment[i].p_memsz;

            if (vaddr_min > first)
                vaddr_min = first;
            if (vaddr_max < last)
                vaddr_max = last;
        }
    }
    return vaddr_max - vaddr_min;
}
#endif

void read_elf32_file(int elf_file, const char *filename, unsigned elf_machine)
{
    //
    // Read ELF header, which is at the beginning of the file.
    //
    Elf32_Ehdr elf_header;
    if (lseek(elf_file, 0, SEEK_SET) != 0) {
        err(-1, "Cannot seek ELF header");
    }
    if (read(elf_file, (char *)&elf_header, sizeof elf_header) != sizeof elf_header) {
        err(-1, "%s: Cannot read ELF header", filename);
    }

    //
    // Make sure ELF header is correct.
    //
    if (elf_header.e_ehsize != sizeof(Elf32_Ehdr)) {
        errexit(-1, ENOEXEC, "Bad ELF header size");
    }
    if (elf_header.e_type != ET_DYN) {
        errexit(-1, ENOEXEC, "Not dynamic");
    }
    if (elf_header.e_machine != elf_machine) {
        errexit(-1, ENOEXEC, "Bad ELF machine %u, expect %u", elf_header.e_machine, elf_machine);
    }
    if (elf_header.e_version != EV_CURRENT) {
        errexit(-1, ENOEXEC, "Bad ELF version");
    }
    if (elf_header.e_phentsize != sizeof(Elf32_Phdr)) {
        errexit(-1, ENOEXEC, "Bad ELF Program Header Entry size");
    }
    if (elf_header.e_phoff == 0) {
        errexit(-1, ENOEXEC, "Bad ELF Program Header offset");
    }
    if (elf_header.e_phnum == 0) {
        errexit(-1, ENOEXEC, "Empty Program headers");
    }
    if (elf_header.e_shnum == 0) {
        errexit(-1, ENOEXEC, "Empty Section headers");
    }
    if (elf_header.e_shentsize != sizeof(Elf32_Shdr)) {
        errexit(-1, ENOEXEC, "Bad Section header size");
    }

    //
    // Read program header.
    //
    Elf32_Phdr segment[elf_header.e_phnum];
    if (lseek(elf_file, elf_header.e_phoff, SEEK_SET) != elf_header.e_phoff) {
        err(-1, "Cannot seek Program header");
    }
    if (read(elf_file, (char *)&segment[0], elf_header.e_phnum * sizeof(Elf32_Phdr)) != elf_header.e_phnum * sizeof(Elf32_Phdr)) {
        err(-1, "Cannot read Program header");
    }
#if 0
    // Total size of the executable.
    unsigned exec_nbytes = find_elf32_size(&elf_header, segment);

    // Allocate virtual memory for the executable via mmap().
    void *exec_buf = mmap(NULL, exec_nbytes, PROT_READ | PROT_WRITE,
                          MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (!exec_buf) {
        err(-1, "Cannot map anonymous memory");
    }

    printf("exec_buf = %p, size %u bytes\n", exec_buf, exec_nbytes);
#endif

    //
    // Read loadable segments.
    //
    for (int i = 0; i < elf_header.e_phnum; i++) {
        // Chooze loadable non-empty segments.
        unsigned vaddr  = segment[i].p_vaddr;
        unsigned memsz  = segment[i].p_memsz;
        unsigned filesz = segment[i].p_filesz;

        switch (segment[i].p_type) {
        case PT_LOAD:
            if (memsz <= 0) {
                // Skip empty sections.
                continue;
            }

            // Verbose logging.
            if (segment[i].p_flags & PF_X) {
                // Executable code.
                printf("Code 0x%x-0x%x size %u bytes\n",
                       vaddr, vaddr + memsz - 1, memsz);
            } else if (filesz > 0) {
                // Initialized data.
                printf("Data 0x%x-0x%x size %u bytes\n",
                       vaddr, vaddr + filesz - 1, filesz);
                if (memsz > filesz) {
                    // Zeroed data.
                    printf("BSS  0x%x-0x%x size %u bytes\n",
                           vaddr + filesz, vaddr + memsz - 1, memsz - filesz);
                }
            }

            // Read data from file.
            if (filesz > 0) {
                if (lseek(elf_file, segment[i].p_offset, SEEK_SET) != segment[i].p_offset) {
                    err(-1, "Cannot seek segment #%u", i);
                }

                char buf[filesz];
                if (read(elf_file, buf, filesz) != filesz) {
                    err(-1, "%s: Cannot read segment #%u", filename, i);
                }
                //TODO: write_output((uint8_t *)buf, vaddr, filesz);
            }

            // Clear BSS.
            if (memsz > filesz) {
                //unsigned nbytes = memsz - filesz;
                //unsigned addr = vaddr + filesz;

                //printf("BSS 0x%x-0x%x size %u bytes\n", addr, addr + nbytes - 1, nbytes);
                //TODO: memset(addr, 0, nbytes);
            }
            break;

        case PT_DYNAMIC:
            //TODO: parse dynamic section
            printf("Dynamic 0x%x-0x%x size %u bytes\n",
                   vaddr, vaddr + memsz - 1, memsz);
            //TODO: parse_elf64_dynamic(exec_buf, vaddr, memsz);
            break;

        case PT_LOPROC + 1:
            // .ARM.exidx section - can be safely ignored.
            break;

        default:
            errexit(-1, ENOEXEC, "Unknown Program Header type 0x%x", segment[i].p_type);
            break;
        }
    }

    //
    // Set PC to the entry point.
    //
    unsigned entry_address = elf_header.e_entry;
    printf("Entry address 0x%jx\n", (uintmax_t)entry_address);
    //TODO: cpu.set_pc(entry_address);

    //
    // Load section headers.
    //
    Elf32_Shdr section_header[elf_header.e_shnum];
    if (lseek(elf_file, elf_header.e_shoff, SEEK_SET) != elf_header.e_shoff) {
        err(-1, "Cannot seek Section header");
    }
    if (read(elf_file, (char *)&section_header[0], elf_header.e_shnum * sizeof(Elf32_Shdr)) != elf_header.e_shnum * sizeof(Elf32_Shdr)) {
        err(-1, "Cannot read Section header");
    }

    //
    // Load Section Header String table.
    //
    unsigned shstr_table_size = section_header[elf_header.e_shstrndx].sh_size;
    char shstr_table[shstr_table_size];
    if (shstr_table_size > 0) {
        // Load the string table.
        if (lseek(elf_file, section_header[elf_header.e_shstrndx].sh_offset, SEEK_SET) != section_header[elf_header.e_shstrndx].sh_offset) {
            err(-1, "Cannot seek Section Header String table");
        }
        if (read(elf_file, shstr_table, shstr_table_size) != shstr_table_size) {
            err(-1, "Cannot read Section Header String table");
        }
    }

    //
    // Load the string table.
    //
    char *string_table = 0;
    unsigned string_table_size = 0;
    for (unsigned i = 0; i < elf_header.e_shnum; i++) {
        if (section_header[i].sh_type == SHT_STRTAB &&
            section_header[i].sh_name < shstr_table_size &&
            strcmp(".strtab", section_header[i].sh_name + shstr_table) == 0) {

            unsigned num_bytes = section_header[i].sh_size;
            if (num_bytes > 0) {
                // Load the string table.
                string_table_size = num_bytes;
                string_table = alloca(num_bytes);
                if (string_table == 0) {
                    errexit(-1, ENOMEM, "Cannot allocate String table");
                }
                if (lseek(elf_file, section_header[i].sh_offset, SEEK_SET) != section_header[i].sh_offset) {
                    err(-1, "Cannot seek String table");
                }
                if (read(elf_file, string_table, num_bytes) != num_bytes) {
                    err(-1, "Cannot read String table");
                }
            }
            break;
        }
    }
    if (string_table == 0) {
        errexit(-1, ENOEXEC, "No String table");
    }

    //
    // Load the symbol table.
    //
    for (unsigned i = 0; i < elf_header.e_shnum; i++) {
        if (section_header[i].sh_type == SHT_SYMTAB) {
            if (section_header[i].sh_entsize != sizeof(Elf32_Sym)) {
                errexit(-1, ENOEXEC, "Bad Symbol Entry size");
            }

            unsigned num_symbols = section_header[i].sh_size / section_header[i].sh_entsize;
            if (num_symbols > 0) {
                // Load the symbol table.
                Elf32_Sym symbol_table[num_symbols];

                if (lseek(elf_file, section_header[i].sh_offset, SEEK_SET) != section_header[i].sh_offset) {
                    err(-1, "Cannot seek Symbol Table");
                }
                if (read(elf_file, (char *)&symbol_table[0], section_header[i].sh_size) != section_header[i].sh_size) {
                    err(-1, "Cannot read Symbol Table");
                }

                for (unsigned s = 0; s < num_symbols; s++) {
                    if (symbol_table[s].st_name != STN_UNDEF &&
                        symbol_table[s].st_name < string_table_size) {
                        // Found a global untyped symbol with valid name.
                        unsigned name_offset = symbol_table[s].st_name;
                        const char *name = string_table + name_offset;

                        printf("Symbol %s = 0x%x\n", name, symbol_table[s].st_value);
                    }
                }
            }
        }
    }
}
