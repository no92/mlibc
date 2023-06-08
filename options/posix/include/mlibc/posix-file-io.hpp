#ifndef MLIBC_POSIX_FILE_IO_HPP
#define MLIBC_POSIX_FILE_IO_HPP

#include <frg/span.hpp>
#include <frg/vector.hpp>
#include <mlibc/file-io.hpp>
#include <mlibc/allocator.hpp>

namespace mlibc {

struct mem_file : abstract_file {
	mem_file(char **ptr, size_t *sizeloc, int flags, void (*do_dispose)(abstract_file *) = nullptr, bool allow_resizing = true);
	mem_file(void *in_buf, size_t size, int flags, void (*do_dispose)(abstract_file *) = nullptr);

	int close() override;
	int reopen(const char *path, const char *mode) override;
protected:
	int determine_type(stream_type *type) override;
	int determine_bufmode(buffer_mode *mode) override;

	int io_read(char *buffer, size_t max_size, size_t *actual_size) override;
	int io_write(const char *buffer, size_t max_size, size_t *actual_size) override;
	int io_seek(off_t offset, int whence, off_t *new_offset) override;
private:
	void _update_ptrs();

	// Convenience function to obtain the correct buffer
	frg::span<char> _buffer() {
		if(_allowResizing) {
			return {_buf.data(), _buffer_size()};
		} else {
			return {reinterpret_cast<char *>(_inBuffer), _buffer_size()};
		}
	}

	// Convenience function to obtain the correct buffer's current size
	size_t _buffer_size() {
		if(_allowResizing) {
			return _buf.size();
		} else {
			return _inBufferSize;
		}
	}

	// Where to write back buffer and size on flush and close.
	char **_bufloc;
	size_t *_sizeloc;

	// Actual resizable buffer (_allowResizing == true).
	frg::vector<char, MemoryAllocator> _buf = {getAllocator()};

	// Provided buffer  (_allowResizing == false).
	void *_inBuffer;
	size_t _inBufferSize;

	off_t _pos = 0;
	int _flags;
	bool _allowResizing = true;
	bool _needsDeallocation = false;
	/* maintains the size of buffer contents as required by POSIX */
	off_t _max_size = 0;
};

struct cookie_file : abstract_file {
	cookie_file(void *cookie, int flags, cookie_io_functions_t funcs, void (*do_dispose)(abstract_file *) = nullptr)
		: abstract_file{do_dispose}, _cookie{cookie}, _flags{flags}, _funcs{funcs} { }

	int close() override;
	int reopen(const char *path, const char *mode) override;
protected:
	int determine_type(stream_type *type) override;
	int determine_bufmode(buffer_mode *mode) override;

	int io_read(char *buffer, size_t max_size, size_t *actual_size) override;
	int io_write(const char *buffer, size_t max_size, size_t *actual_size) override;
	int io_seek(off_t offset, int whence, off_t *new_offset) override;

private:
	void *_cookie;

	int _flags;
	cookie_io_functions_t _funcs;
};

} // namespace mlibc

#endif // MLIBC_POSIX_FILE_IO_HPP
