#include "asm-generic/ioctl.h"
#include "fs.frigg_bragi.hpp"
#include "mlibc/allocator.hpp"
#include "mlibc/posix-pipe.hpp"

#include <drm/drm.h>
#include <drm/virtgpu_drm.h>
#include <errno.h>
#include <hel.h>

namespace mlibc {

int ioctl_drm_virtio(int fd, unsigned long request, void *arg, int *result, HelHandle handle);

int ioctl_drm_driver_specific(int fd, unsigned long request, void *arg, int *result, HelHandle handle) {
	managarm::fs::IoctlRequest<MemoryAllocator> ioctl_req(getSysdepsAllocator());
	managarm::fs::DrmCardDriverRequest<MemoryAllocator> driver_req{getSysdepsAllocator()};

	auto [offer, send_ioctl_req, send_req, recv_resp] = exchangeMsgsSync(
		handle,
		helix_ng::offer(
			helix_ng::sendBragiHeadOnly(ioctl_req, getSysdepsAllocator()),
			helix_ng::sendBragiHeadOnly(driver_req, getSysdepsAllocator()),
			helix_ng::recvInline()
	));
	HEL_CHECK(offer.error());
	HEL_CHECK(send_ioctl_req.error());
	HEL_CHECK(send_req.error());
	HEL_CHECK(recv_resp.error());

	managarm::fs::DrmCardDriverReply<MemoryAllocator> resp{getSysdepsAllocator()};
	resp.ParseFromArray(recv_resp.data(), recv_resp.length());

	if(resp.driver_id() == "virtio_gpu") {
		return ioctl_drm_virtio(fd, request, arg, result, handle);
	} else {
		mlibc::infoLogger() << "mlibc: DRM driver-specific ioctls " << resp.driver_id()
			<< " are unimplemented!" << frg::endlog;
		return ENOSYS;
	}
}

int ioctl_drm_virtio(int fd, unsigned long request, void *arg, int *result, HelHandle handle) {
	managarm::fs::IoctlRequest<MemoryAllocator> ioctl_req(getSysdepsAllocator());
	managarm::fs::DrmDriverIoctl<MemoryAllocator> ioctl_driver_req(getSysdepsAllocator());
	auto num = _IOC_NR(request) - DRM_COMMAND_BASE;

	switch(num) {
		case DRM_VIRTGPU_MAP: {
			auto param = reinterpret_cast<drm_virtgpu_map *>(arg);
			managarm::fs::DrmIoctlVirtioMapRequest<MemoryAllocator> req{getSysdepsAllocator()};
			req.set_handle(param->handle);

			auto [offer, send_ioctl_req, send_ioctl_driver_req, send_req, recv_resp] =
			exchangeMsgsSync(
				handle,
				helix_ng::offer(
					helix_ng::sendBragiHeadOnly(ioctl_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(ioctl_driver_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(req, getSysdepsAllocator()),
					helix_ng::recvInline()
			));
			HEL_CHECK(offer.error());
			HEL_CHECK(send_ioctl_req.error());
			HEL_CHECK(send_ioctl_driver_req.error());
			HEL_CHECK(send_req.error());
			HEL_CHECK(recv_resp.error());

			managarm::fs::DrmIoctlVirtioMapReply<MemoryAllocator> resp{getSysdepsAllocator()};
			resp.ParseFromArray(recv_resp.data(), recv_resp.length());

			param->offset = resp.offset();

			break;
		}
		case DRM_VIRTGPU_GETPARAM: {
			auto param = reinterpret_cast<drm_virtgpu_getparam *>(arg);
			managarm::fs::DrmIoctlVirtioGetParamRequest<MemoryAllocator> req{getSysdepsAllocator()};
			req.set_param(param->param);

			auto [offer, send_ioctl_req, send_ioctl_driver_req, send_req, recv_resp] =
			exchangeMsgsSync(
				handle,
				helix_ng::offer(
					helix_ng::sendBragiHeadOnly(ioctl_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(ioctl_driver_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(req, getSysdepsAllocator()),
					helix_ng::recvInline()
			));
			HEL_CHECK(offer.error());
			HEL_CHECK(send_ioctl_req.error());
			HEL_CHECK(send_ioctl_driver_req.error());
			HEL_CHECK(send_req.error());
			HEL_CHECK(recv_resp.error());

			managarm::fs::DrmIoctlVirtioGetParamReply<MemoryAllocator> resp{getSysdepsAllocator()};
			resp.ParseFromArray(recv_resp.data(), recv_resp.length());
			*reinterpret_cast<uintptr_t *>(param->value) = resp.value();
			break;
		}
		case DRM_VIRTGPU_RESOURCE_CREATE: {
			auto param = reinterpret_cast<drm_virtgpu_resource_create *>(arg);
			managarm::fs::DrmIoctlVirtioCreateResourceRequest<MemoryAllocator> req{getSysdepsAllocator()};
			req.set_target(param->target);
			req.set_format(param->format);
			req.set_bind(param->bind);
			req.set_width(param->width);
			req.set_height(param->height);
			req.set_depth(param->depth);
			req.set_array_size(param->array_size);
			req.set_last_level(param->last_level);
			req.set_nr_samples(param->nr_samples);
			req.set_flags(param->flags);
			req.set_size(param->size);
			req.set_stride(param->stride);

			auto [offer, send_ioctl_req, send_ioctl_driver_req, send_req, recv_resp] =
			exchangeMsgsSync(
				handle,
				helix_ng::offer(
					helix_ng::sendBragiHeadOnly(ioctl_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(ioctl_driver_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(req, getSysdepsAllocator()),
					helix_ng::recvInline()
			));
			HEL_CHECK(offer.error());
			HEL_CHECK(send_ioctl_req.error());
			HEL_CHECK(send_ioctl_driver_req.error());
			HEL_CHECK(send_req.error());
			HEL_CHECK(recv_resp.error());

			managarm::fs::DrmIoctlVirtioCreateResourceReply<MemoryAllocator> resp{getSysdepsAllocator()};
			resp.ParseFromArray(recv_resp.data(), recv_resp.length());

			param->bo_handle = resp.bo_handle();
			param->res_handle = resp.res_handle();

			break;
		}
		case DRM_VIRTGPU_GET_CAPS: {
			auto param = reinterpret_cast<drm_virtgpu_get_caps *>(arg);
			managarm::fs::DrmIoctlVirtioGetCapsRequest<MemoryAllocator> req{getSysdepsAllocator()};
			req.set_id(param->cap_set_id);
			req.set_version(param->cap_set_ver);
			req.set_size(param->size);

			auto [offer, send_ioctl_req, send_ioctl_driver_req, send_req, recv_resp] =
			exchangeMsgsSync(
				handle,
				helix_ng::offer(
					helix_ng::want_lane,
					helix_ng::sendBragiHeadOnly(ioctl_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(ioctl_driver_req, getSysdepsAllocator()),
					helix_ng::sendBragiHeadOnly(req, getSysdepsAllocator()),
					helix_ng::recvInline()
			));
			HEL_CHECK(offer.error());
			HEL_CHECK(send_ioctl_req.error());
			HEL_CHECK(send_ioctl_driver_req.error());
			HEL_CHECK(send_req.error());
			HEL_CHECK(recv_resp.error());

			managarm::fs::DrmIoctlVirtioGetParamReply<MemoryAllocator> resp{getSysdepsAllocator()};
			resp.ParseFromArray(recv_resp.data(), recv_resp.length());

			auto [recv_buf] = exchangeMsgsSync(offer.descriptor().getHandle(),
				helix_ng::recvBuffer(reinterpret_cast<void *>(param->addr), param->size)
			);

			HEL_CHECK(recv_buf.error());

			break;
		}
		default: {
			mlibc::infoLogger() << "mlibc: DRM virtio-gpu ioctl " << num
				<< " is unhandled!" << frg::endlog;
			return ENOSYS;
		}
	}

	return 0;
}

} // namespace mlibc
