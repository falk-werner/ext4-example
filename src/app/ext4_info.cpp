#include "ext4/ext4.hpp"

#include <string>
#include <iostream>

namespace
{

char const * get_state(uint16_t state)
{
    switch (state)
    {
        case ext4::fs_state_clean:
            return "clean";
        case ext4::fs_state_haserrors:
            return "has errors"; 
        default:
            return "<unknown>";
    }
}

char const * get_errors(uint16_t errors)
{
    switch (errors)
    {
        case ext4::fs_errors_ignore:
            return "ignore";
        case ext4::fs_errors_remount_ro:
            return "remount read-only";
        case ext4::fs_errors_panic:
            return "panic";
        default:
            return "<unknown>";
    }
}

char const * get_creator_os(uint32_t creator_os)
{
    switch (creator_os)
    {
        case ext4::fs_creator_os_linux:
            return "Linux";
        case ext4::fs_creator_os_gnu_hurd:
            return "GNU Hurd";
        case ext4::fs_creator_os_masix:
            return "MASIX";
        case ext4::fs_creator_os_freebsd:
            return "FreeBSD";
        case ext4::fs_creator_os_lite:
            return "BSD4.4-Lite derivative";
        default:
            return "<unknown>";
    }
}

struct known_feature
{
    uint32_t id;
    std::string description;
};

known_feature const compatible_features[] =
{
    {ext4::fs_compat_dir_prealloc, "DIR_PREALLOC: directory preallocation"},
    {ext4::fs_compat_imagic_inodes, "IMAGIC_INODES: image inodes"},
    {ext4::fs_compat_has_journal, "HAS_JOUNRAL: has a journal"},
    {ext4::fs_compat_ext_attr, "EXT_ATTR: supports extended attributes"},
    {ext4::fs_compat_resize_inode, "RESIZE_INODE: has reserve GDT blocks for filesystem expansion. Requires RO_COMPAT_SPARSE_SUPER"},
    {ext4::fs_compat_dir_index, "DIR_INDEX: has directory indices"},
    {ext4::fs_compat_lazy_bg, "LAZY_BG: Lazy BG"},
    {ext4::fs_compat_exclude_inode, "EXCLUDE_INODE: Exclude inode"},
    {ext4::fs_compat_exclude_bitmap, "EXCLUDE_BITMAP: Exclude bitmap"},
    {ext4::fs_compat_sparse_super2, "SPARSE_SUPER2: Sparse Super Block v2"},
    {ext4::fs_compat_fast_commit, "FAST_COMMIT: Fast commits supported."},
    {ext4::fs_compat_orphan_present, "ORPHAN_PRESENT: Orphan file allocated"},
    {0, ""}
};

known_feature const incompatible_features[] =
{
    {ext4::fs_incompat_compression, "COMPRESSION: Compression"},
    {ext4::fs_incompat_filetype, "FILETYPE: Directory entries record the file type."},
    {ext4::fs_incompat_recover, "RECOVER: Filesystem needs recovery."},
    {ext4::fs_incompat_journal_dev, "JOURNAL_DEV: Filesystem has separate journal device."},
    {ext4::fs_incompat_meta_bg, "META_BG: Meta block groups"},
    {ext4::fs_incompat_extents, "EXTENDS: Files in this filesystem us extents."},
    {ext4::fs_incompat_64bit, "64BIT: enable a filesystem size of 2^64 blocks."},
    {ext4::fs_incompat_mmp, "MMP: multiple mount protection"},
    {ext4::fs_incompat_flex_bg, "FLEX_BG: Flexible block groups."},
    {ext4::fs_incompat_ea_inode, "EA_INODE: Inodes can be used to store large extended attribute values"},
    {ext4::fs_incompat_dirdata, "DIR_DATA: Data in directory entry"},
    {ext4::fs_incompat_csum_seed, "CSUM_SEED: Metadata checksum seed is stored in superblock."},
    {ext4::fs_incompat_largedir, "LARGEDIR: Large directory > 2GB or 3-level htree"},
    {ext4::fs_incompat_inline_data, "INLINE_DATA: Data in inode"},
    {ext4::fs_incompat_encrypt, "ENCRYPT: Encrypted inodes are present on the filesystem"},
    {0, ""}
};

known_feature const ro_compatible_features[] =
{
    {ext4::fs_ro_compat_sparse_super, "SPARSE_SUPER: Sparse superblocks"},
    {ext4::fs_ro_compat_large_file, "LARGE_FILE: This filesystem has been used to store a file greater that 2 GB"},
    {ext4::fs_ro_compat_btree_dir, "BTREE_DIR: BTree Dir"},
    {ext4::fs_ro_compat_huge_file, "HUGE_FILE: This filesystem has files whose sizes are represented in units of logical blocks, not 512-byte sectors"},
    {ext4::fs_ro_compat_gdt_csum, "GDT_CSUM: Group descriptors have checksums"},
    {ext4::fs_ro_compat_dir_nlink, "DIR_NLINK: Indicates that the old ext3 32,000 subdirectory limit no longer applies"},
    {ext4::fs_ro_compat_extra_isize, "EXTRA_ISIZE: Indicates that large indoes exist on this filesystem"},
    {ext4::fs_ro_compat_has_snapshot, "HAS_SNAPSHOT: This filesystem has a snapshot"},
    {ext4::fs_ro_compat_quota, "QUOTA: Quota"},
    {ext4::fs_ro_compat_bigalloc, "BIGALLOC: This filesystem supports \"bigalloc\""},
    {ext4::fs_ro_compat_metadata_csum, "METADATA_CSUM: This filesystem supprots metadata checkusmming."},
    {ext4::fs_ro_compat_replica, "REPLICA: This filesystem supports replicas."},
    {ext4::fs_ro_compat_readonly, "READONLY: Read-only filesystem image."},
    {ext4::fs_ro_compat_project, "PROJECT: Filesystem track project quotas."},
    {ext4::fs_ro_compat_verity, "VERITY: Verity inodes may be present on the filesystem."},
    {ext4::fs_ro_compat_orphan_present, "ORPHAN_PRESENT: Indicates orphan files may have valid orphan entries and thus we need to clean them up when mounting the filesystem."},
    {0, ""}
};

void print_features(uint32_t features, known_feature const * feature_set)
{
    for (size_t i = 0; feature_set[i].id != 0; i++)
    {
        if ((features & feature_set[i].id) != 0)
        {
            std::cout << "  " << feature_set[i].description << std::endl;
        }
    }
}

char hex_char(int value)
{
    static char const map[] = "0123456789abcdef";
    return ((0 <= value) && (value <= 15)) ? map[value] : '?';
}

void print_hex(char c)
{
    int const low = c & 0xf;
    int const high = (c >> 4) & 0xf;
    std::cout << hex_char(high) << hex_char(low);
}

void print_uuid(std::string const & value)
{
    print_hex(value.at(0));
    print_hex(value.at(1));
    print_hex(value.at(2));
    print_hex(value.at(3));
    std::cout << '-';
    print_hex(value.at(4));
    print_hex(value.at(5));
    std::cout << '-';
    print_hex(value.at(6));
    print_hex(value.at(7));
    std::cout << '-';
    print_hex(value.at(8));
    print_hex(value.at(9));
    print_hex(value.at(10));
    print_hex(value.at(11));
    print_hex(value.at(12));
    print_hex(value.at(13));
    print_hex(value.at(14));
    print_hex(value.at(15));
}

}

int main(int argc, char* argv[])
{
    int exit_code = EXIT_SUCCESS;

    if (argc > 1)
    {
        try
        {
            std::string const filename = argv[1];
            auto fs = ext4::open(filename);

            ext4::fsinfo info;
            fs->get_info(info);

            std::cout << "[ superblock.basic ]" << std::endl;
            std::cout << "total blocks: " << info.total_blocks << std::endl;
            std::cout << "total inodes: " << info.total_inodes << std::endl;
            std::cout << "reserved blocks: " << info.reserved_blocks << std::endl;
            std::cout << "free blocks: " << info.free_blocks << std::endl;
            std::cout << "free inodes: " << info.free_inodes << std::endl;
            std::cout << "first data block: " << info.first_data_block << std::endl;
            std::cout << "block size: " << info.block_size << std::endl;
            std::cout << "blocks per group: " << info.blocks_per_group << std::endl;
            std::cout << "inodes per group: " << info.inodes_per_group << std::endl;
            std::cout << "state: " << get_state(info.state) << " (" << info.state << ')' << std::endl;
            std::cout << "errors: " << get_errors(info.errors) << " (" << info.errors << ')' << std::endl;
            std::cout << "revision: "  << info.revision << '.' << info.minor_revision << std::endl;
            std::cout << "creator os: " << get_creator_os(info.creator_os) << " (" << info.creator_os << ')' << std::endl;
            std::cout << std::endl;
            std::cout << "[ superblock.extended ]" << std::endl;
            std::cout << "first inode: " << info.first_ino << std::endl;
            std::cout << "inode size: " << info.inode_size << std::endl;
            std::cout << "compatible features: 0x" << std::hex << info.feature_compatible << std::dec << std::endl;
            print_features(info.feature_compatible, compatible_features);
            std::cout << "incompatible features: 0x" << std::hex << info.feature_incompatible << std::dec << std::endl;
            print_features(info.feature_incompatible, incompatible_features);
            std::cout << "ready-only compatible features: 0x" << std::hex << info.feature_ro_compatible << std::dec << std::endl;
            print_features(info.feature_ro_compatible, ro_compatible_features);
            std::cout << "blockgrounp descriptor size: " << info.blockgroup_descriptor_size << std::endl;
            std::cout << "uuid: "; print_uuid(info.uuid); std::cout << std::endl;
            std::cout << "volume name: " << info.volume_name << std::endl;

        }
        catch (std::exception const & ex)
        {
            std::cerr << "error: " << ex.what() << std::endl;
        }
        catch (...)
        {
            std::cerr << "error: fatal error" << std::endl;
        }
    }
    else
    {
        std::cout << "usage: ext4-info <filename>" << std::endl;
    }

    return exit_code;
}