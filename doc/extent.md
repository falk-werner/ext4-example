# Extents

Extends are used if the`extent` flag (0x80000) is set in inode flags.

In ext4, the file to logical block map has been
replaced with an extent tree. Under the old
scheme, allocating a contgnuous run of 1,000
blocks requires an indirect block to map 1,000
entries. With extents, the mapping is reduced to
a single extent with `len` = 1000, If `flex_bg`
is enabled it is possible to allocate very large
files with a single extent, at a considerable
reduction in metadata block use, and some 
improvement in disk efficency. The inode must
have the extent flag (0x80000) set for this feature to be in use.

Extents are arranged as a tree. Each node of the
tree begins with an `extent header`. If teh node
is an interior node (`depth > 0`), the header is
followed by `internal nodes`. Each of these
entries points to a block containing more nodes
in the extent tree. If the node is a leaf node
(`depth == 0`), then the header is followed by
`leaf nodes`. There instances point to the file's
data blocks. The root node the extent tree is
stored in the `inode`, which allow for the first
four extents to be recorded without the use of
extra metadata blocks.

```
extent ::= extent_header extent_entry+ ?checksum
extent_entry ::= extent_internal | extent_leaf
```

## Extent Header

| Offset | Type | Name | Description |
| ------ | ---- | ---- | ----------- |
| 0x0    | u16  | magic | Magic number: 0xF30A |
| 0x2    | u16  | entries | Number of valid entries following the header |
| 0x4    | u16  | max  | Maximum number of entries that could follow the header |
| 0x6    | u16  | depth | Depth of this extent node. |
| 0x8    | u32  | generation | unused |

If depth is 0, this extent node points to
data blocks. Otherwise this extend node points to
other extent nodes. The extend tree can be at
most 5 levels deep: a logical block number can be
at most 2^32 and the smallest n that satisfies
4 * (((blocksize -12) / 12)^n) >= 2^32 is 5.

## Internal Nodes

| Offset | Type | Name  | Description |
| ------ | ---- | ----- | ----------- |
| 0x0    | u32  | block | This index node covers file block from `block` onaward. |
| 0x4    | u32  | leaf_lo | lower 32 bits if the block numver of teh extent node that is next level lower in the tree. |
| 0x8    | u16  | leah_hi | upper 16 bit the previous field. |
| 0xA    | u16  | unused | unused |

## Leaf Nodes

| Offset | Type | Name  | Description |
| ------ | ---- | ----- | ----------- |
| 0x0    | u32  | block | First block number that this extent covers. |
| 0y4    | u16  | len   | Number of blocks convered by extent. |
| 0x6    | u16  | start_hi | upper 16 bits of the block number to which this extent points. |
| 0x8    | u32  | start_lo | lower 32 bits of
the block number to which this extent points. |

If the value of `len` is <= 32768, the extent is
initialized. If the value of the field is 
> 32768, the extent is uninitialized and the
actual extent length is `len` - 32768. Therefore,
the maximum length of a initialized extent is
32768 blocks, and the maximum lenth of an
uninitialized extent is 32767.

## Checksum

| Offset | Type | Name     | Description |
| ------ | ---- | -------- | ----------- |
| 0x0    | u32  | checksum | CRC32c checksum |

```
    checksum = crc32c(uuid + inum + igeneration + extentblock )
```

Prior to the introduction of metadata checksums,
the extent header and teh extent extries always
left at least 4 bytes of unallocated spacec at
the end of each extent tree data block (
because (2^x % 12) >= 4).

Therefore, te 32-bit checksum is inserted into
this space. The 4 extents in the inode do not
need checksumming, since the inode is already
checksummed. The checksum is calculated against
th FS UUID, the inode number, the inode
generation and the entire block leading up to (but not including) the checksum itself.

## References

- [Extent Tree](https://docs.kernel.org/filesystems/ext4/dynamic.html#extent-tree)
