#ifndef EXT4_FSINFO_HPP
#define EXT4_FSINFO_HPP

namespace ext4
{

constexpr uint16_t const fs_state_clean = 1;
constexpr uint16_t const fs_state_haserrors = 2;

constexpr uint16_t const fs_errors_ignore = 1;
constexpr uint16_t const fs_errors_remount_ro = 2;
constexpr uint16_t const fs_errors_panic = 3;

constexpr uint32_t const fs_creator_os_linux = 0;
constexpr uint32_t const fs_creator_os_gnu_hurd = 1;
constexpr uint32_t const fs_creator_os_masix = 2;
constexpr uint32_t const fs_creator_os_freebsd = 3;
constexpr uint32_t const fs_creator_os_lite = 4;

constexpr uint32_t const fs_compat_dir_prealloc   = 0x0001;
constexpr uint32_t const fs_compat_imagic_inodes  = 0x0002;
constexpr uint32_t const fs_compat_has_journal    = 0x0004;
constexpr uint32_t const fs_compat_ext_attr       = 0x0008;
constexpr uint32_t const fs_compat_resize_inode   = 0x0010;
constexpr uint32_t const fs_compat_dir_index      = 0x0020;
constexpr uint32_t const fs_compat_lazy_bg        = 0x0040;
constexpr uint32_t const fs_compat_exclude_inode  = 0x0080;
constexpr uint32_t const fs_compat_exclude_bitmap = 0x0100;
constexpr uint32_t const fs_compat_sparse_super2  = 0x0200;
constexpr uint32_t const fs_compat_fast_commit    = 0x0400;
constexpr uint32_t const fs_compat_orphan_present = 0x1000;

constexpr uint32_t const fs_incompat_compression = 0x00001;
constexpr uint32_t const fs_incompat_filetype    = 0x00002;
constexpr uint32_t const fs_incompat_recover     = 0x00004;
constexpr uint32_t const fs_incompat_journal_dev = 0x00008;
constexpr uint32_t const fs_incompat_meta_bg     = 0x00010;
constexpr uint32_t const fs_incompat_extents     = 0x00040;
constexpr uint32_t const fs_incompat_64bit       = 0x00080;
constexpr uint32_t const fs_incompat_mmp         = 0x00100;
constexpr uint32_t const fs_incompat_flex_bg     = 0x00200;
constexpr uint32_t const fs_incompat_ea_inode    = 0x00400;
constexpr uint32_t const fs_incompat_dirdata     = 0x01000;
constexpr uint32_t const fs_incompat_csum_seed   = 0x02000;
constexpr uint32_t const fs_incompat_largedir    = 0x04000;
constexpr uint32_t const fs_incompat_inline_data = 0x08000;
constexpr uint32_t const fs_incompat_encrypt     = 0x10000;

constexpr uint32_t const fs_ro_compat_sparse_super   = 0x00001;
constexpr uint32_t const fs_ro_compat_large_file     = 0x00002;
constexpr uint32_t const fs_ro_compat_btree_dir      = 0x00004;
constexpr uint32_t const fs_ro_compat_huge_file      = 0x00008;
constexpr uint32_t const fs_ro_compat_gdt_csum       = 0x00010;
constexpr uint32_t const fs_ro_compat_dir_nlink      = 0x00020;
constexpr uint32_t const fs_ro_compat_extra_isize    = 0x00040;
constexpr uint32_t const fs_ro_compat_has_snapshot   = 0x00080;
constexpr uint32_t const fs_ro_compat_quota          = 0x00100;
constexpr uint32_t const fs_ro_compat_bigalloc       = 0x00200;
constexpr uint32_t const fs_ro_compat_metadata_csum  = 0x00400;
constexpr uint32_t const fs_ro_compat_replica        = 0x00800;
constexpr uint32_t const fs_ro_compat_readonly       = 0x01000;
constexpr uint32_t const fs_ro_compat_project        = 0x02000;
constexpr uint32_t const fs_ro_compat_verity         = 0x08000;
constexpr uint32_t const fs_ro_compat_orphan_present = 0x10000;

struct fsinfo
{
    uint64_t total_blocks;
    uint32_t total_inodes;
    uint64_t reserved_blocks;
    uint64_t free_blocks;
    uint32_t free_inodes;
    uint64_t first_data_block;
    uint64_t block_size;
    uint32_t blocks_per_group;
    uint32_t inodes_per_group;
    uint16_t state;
    uint16_t errors;
    uint16_t minor_revision;
    uint32_t creator_os;
    uint32_t revision;

    uint32_t first_ino;
    uint32_t inode_size;
    uint32_t feature_compatible;
    uint32_t feature_incompatible;
    uint32_t feature_ro_compatible;
    std::string uuid;
    std::string volume_name;

    uint16_t blockgroup_descriptor_size;
};

}

#endif
