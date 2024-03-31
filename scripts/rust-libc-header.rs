// This file is autogenerated!
// All changes made will be lost (eventually)!

use sighandler_t;

pub type c_char = i8;
pub type c_long = i64;
pub type c_ulong = u64;
pub type blkcnt64_t = i64;
pub type rlimit64 = ::rlimit;
pub type rlim64_t = ::rlim_t;
pub type ino64_t = ::ino_t;
pub type dirent64 = ::dirent;
pub type stat64 = ::stat;
pub type statfs64 = ::statfs;
pub type statvfs64 = ::statvfs;
pub type idtype_t = ::c_uint;
pub const RTLD_DEFAULT: *mut ::c_void = 0i64 as *mut ::c_void;
pub const RLIM_INFINITY: ::rlim_t = !0;

pub type Elf32_Half = u16;
pub type Elf32_Word = u32;
pub type Elf32_Off = u32;
pub type Elf32_Addr = u32;

pub type Elf64_Half = u16;
pub type Elf64_Word = u32;
pub type Elf64_Off = u64;
pub type Elf64_Addr = u64;
pub type Elf64_Xword = u64;

s! {
    pub struct Elf32_Phdr {
        pub p_type: Elf32_Word,
        pub p_offset: Elf32_Off,
        pub p_vaddr: Elf32_Addr,
        pub p_paddr: Elf32_Addr,
        pub p_filesz: Elf32_Word,
        pub p_memsz: Elf32_Word,
        pub p_flags: Elf32_Word,
        pub p_align: Elf32_Word,
    }

    pub struct Elf64_Phdr {
        pub p_type: Elf64_Word,
        pub p_flags: Elf64_Word,
        pub p_offset: Elf64_Off,
        pub p_vaddr: Elf64_Addr,
        pub p_paddr: Elf64_Addr,
        pub p_filesz: Elf64_Xword,
        pub p_memsz: Elf64_Xword,
        pub p_align: Elf64_Xword,
    }
}

s! {
    pub struct dl_phdr_info {
        #[cfg(target_pointer_width = "64")]
        pub dlpi_addr: Elf64_Addr,
        #[cfg(target_pointer_width = "32")]
        pub dlpi_addr: Elf32_Addr,

        pub dlpi_name: *const ::c_char,

        #[cfg(target_pointer_width = "64")]
        pub dlpi_phdr: *const Elf64_Phdr,
        #[cfg(target_pointer_width = "32")]
        pub dlpi_phdr: *const Elf32_Phdr,

        #[cfg(target_pointer_width = "64")]
        pub dlpi_phnum: Elf64_Half,
        #[cfg(target_pointer_width = "32")]
        pub dlpi_phnum: Elf32_Half,

        pub dlpi_adds: ::c_ulonglong,
        pub dlpi_subs: ::c_ulonglong,
        pub dlpi_tls_modid: ::size_t,
        pub dlpi_tls_data: *mut ::c_void,
    }
}

f! {
    pub fn CMSG_NXTHDR(mhdr: *const msghdr, cmsg: *const cmsghdr) -> *mut cmsghdr {
        if ((*cmsg).cmsg_len as usize) < ::mem::size_of::<cmsghdr>() {
            return 0 as *mut cmsghdr;
        };
        let next = (cmsg as usize + super::CMSG_ALIGN((*cmsg).cmsg_len as usize)) as *mut cmsghdr;
        let max = (*mhdr).msg_control as usize + (*mhdr).msg_controllen as usize;
        if (next.offset(1)) as usize > max ||
            next as usize + super::CMSG_ALIGN((*next).cmsg_len as usize) > max {
            0 as *mut cmsghdr
        } else {
            next as *mut cmsghdr
        }
    }
}

align_const! {
    pub const PTHREAD_MUTEX_INITIALIZER: pthread_mutex_t = pthread_mutex_t {
        size: [0; 16],
    };
    pub const PTHREAD_COND_INITIALIZER: pthread_cond_t = pthread_cond_t {
        size: [0; 12],
    };
    pub const PTHREAD_RWLOCK_INITIALIZER: pthread_rwlock_t = pthread_rwlock_t {
        size: [0; 12],
    };
}
s_no_extra_traits! {
    pub struct ifreq {
        pub ifru_addr: ::sockaddr,
        pub ifru_dstaddr: ::sockaddr,
        pub ifru_broadaddr: ::sockaddr,
        pub ifru_netmask: ::sockaddr,
        pub ifru_hwaddr: ::sockaddr,
        pub ifru_flags: ::c_short,
        pub ifru_ivalue: ::c_int,
        pub ifru_mtu: ::c_int,
        pub ifru_map: ::ifmap,
        pub ifru_slave: [::c_char; 16],
        pub ifru_newname: [::c_char; 16],
        pub ifru_data: *mut ::c_char,
    }
}

impl ::fmt::Debug for ifreq {
    fn fmt(&self, f: &mut ::fmt::Formatter<'_>) -> ::fmt::Result {
        f.debug_struct("ifreq").finish()
    }
}

safe_f! {
    pub {const} fn makedev(major: ::c_uint, minor: ::c_uint) -> ::dev_t {
        let major = major as ::dev_t;
        let minor = minor as ::dev_t;
        let mut dev = 0;
        dev |= (major & 0x00000fff) << 8;
        dev |= (major & 0xfffff000) << 32;
        dev |= (minor & 0x000000ff) << 0;
        dev |= (minor & 0xffffff00) << 12;
        dev
    }
}

f! {
    pub fn major(dev: ::dev_t) -> ::c_uint {
        let mut major = 0;
        major |= (dev & 0x00000000000fff00) >> 8;
        major |= (dev & 0xfffff00000000000) >> 32;
        major as ::c_uint
    }

    pub fn minor(dev: ::dev_t) -> ::c_uint {
        let mut minor = 0;
        minor |= (dev & 0x00000000000000ff) >> 0;
        minor |= (dev & 0x00000ffffff00000) >> 12;
        minor as ::c_uint
    }
}
