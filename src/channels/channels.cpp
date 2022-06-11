#include "channels.h"

namespace pdsChannels
{
    blc_channel state("/pdsDrone.state", BLC_CHANNEL_READ);
    blc_channel highresImu("/pdsDrone.sensors.highres", BLC_CHANNEL_READ);
    blc_channel heartbeat("/pdsDrone.heartbeat", BLC_CHANNEL_READ);
    blc_channel altitude("/pdsDrone.sensors.altitude", BLC_CHANNEL_READ);
    blc_channel attitude("/pdsDrone.sensors.attitude", BLC_CHANNEL_READ);
    blc_channel localPositionNed("/pdsDrone.sensors.local_position", BLC_CHANNEL_READ);
    blc_channel globalPosition("/pdsDrone.sensors.global_position", BLC_CHANNEL_READ);
    blc_channel battery("/pdsDrone.sensors.battery", BLC_CHANNEL_READ);
    blc_channel image("/pdsDrone.camera.image", BLC_CHANNEL_READ);
    blc_channel imageSize("/pdsDrone.camera.image_size", BLC_CHANNEL_READ);
}