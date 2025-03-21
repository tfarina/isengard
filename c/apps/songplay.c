#define MINIAUDIO_IMPLEMENTATION
#include "third_party/miniaudio/miniaudio.h"

#include <stdio.h>

void data_callback(ma_device* pDevice, void* pOutput, const void* pInput, ma_uint32 frameCount)
{
  ma_decoder* pDecoder = (ma_decoder*)pDevice->pUserData;
  if (NULL == pDecoder)
  {
    return;
  }

  ma_decoder_read_pcm_frames(pDecoder, pOutput, frameCount, NULL);

  (void)pInput;
}

int
main(int argc, char **argv)
{
  ma_result result;
  ma_decoder decoder;
  ma_device_config deviceConfig;
  ma_device device;

  if (argc < 2)
  {
    fputs("Usage: songplay <filename.mp3>\n", stderr);
    return -1;
  }

  result = ma_decoder_init_file(argv[1], NULL, &decoder);
  if (MA_SUCCESS != result)
  {
    fprintf(stderr, "Could not load file: %s\n", argv[1]);
    return -2;
  }

  deviceConfig = ma_device_config_init(ma_device_type_playback);
  deviceConfig.playback.format = decoder.outputFormat;
  deviceConfig.playback.channels = decoder.outputChannels;
  deviceConfig.sampleRate = decoder.outputSampleRate;
  deviceConfig.dataCallback = data_callback;
  deviceConfig.pUserData = &decoder;

  result = ma_device_init(NULL, &deviceConfig, &device);
  if (MA_SUCCESS != result)
  {
    fputs("Failed to open playback device.\n", stderr);
    ma_decoder_uninit(&decoder);
    return -3;
  }

  result = ma_device_start(&device);
  if (MA_SUCCESS != result)
  {
    fputs("Failed to start playback device.\n", stderr);
    ma_device_uninit(&device);
    ma_decoder_uninit(&decoder);
    return -4;
  }

  fputs("Press Enter to quit...", stdout);
  getchar();

  ma_device_uninit(&device);
  ma_decoder_uninit(&decoder);

  return 0;
}
