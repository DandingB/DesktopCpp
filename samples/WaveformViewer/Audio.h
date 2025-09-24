#pragma once
#include <vector>
#include <string>

struct WAV_RIFF
{
	uint32_t ChunkID;
	uint32_t ChunkSize;
	uint32_t Format;
};

struct WAV_FMT
{
	uint32_t SubChunk1ID;
	uint32_t SubChunk1Size;
	uint16_t AudioFormat;
	uint16_t NumChannels;
	uint32_t SampleRate;
	uint32_t ByteRate;
	uint16_t BlockAlign;
	uint16_t BitsPerSample;
};

struct WAV_DATA
{
	uint32_t SubChunk1ID;
	uint32_t SubChunk2Size;
};

class AudioPeakData
{
public:
	struct Peak
	{
		float high, low;
	};

	std::vector<Peak> m_Peaks;
};

void LoadWaveFile(std::wstring path, AudioPeakData& out)
{
	WAV_RIFF _riff = {};
	WAV_FMT _fmt = {};
	WAV_DATA _data = {};
	std::vector<uint8_t> data;
	std::vector<std::vector<float>> audio;

	FILE* file = _wfopen(path.c_str(), L"rb");
	fread(&_riff, sizeof(WAV_RIFF), 1, file);
	fread(&_fmt, sizeof(WAV_FMT), 1, file);
	fread(&_data, sizeof(WAV_DATA), 1, file);

	data.resize(_data.SubChunk2Size);
	fread(data.data(), _data.SubChunk2Size, 1, file);

	uint32_t nSamples = _data.SubChunk2Size / _fmt.BlockAlign;

	audio.resize(_fmt.NumChannels);
	for (uint32_t c = 0; c < _fmt.NumChannels; c++)
		audio[c].resize(nSamples);

	uint32_t CurrentSample = 0;
	for (uint32_t n = 0; n < nSamples; n++)
	{
		for (uint32_t c = 0; c < _fmt.NumChannels; c++)
		{
			float sample;
			memcpy(&sample, &data[CurrentSample * sizeof(float)], 4);
			audio[c][n] = sample;

			CurrentSample++;
		}
	}

	uint32_t MIPSample = 0;
	float highest = 0;
	float lowest = 0;

	for (int i = 0; i < nSamples; i++)
	{
		if (MIPSample == 0)
		{
			highest = audio[0][i];
			lowest = audio[0][i];
		}
		else
		{
			if (audio[0][i] > highest) { highest = audio[0][i]; }
			if (audio[0][i] < lowest) { lowest = audio[0][i]; }
		}

		if (MIPSample == 512 - 1)
		{
			out.m_Peaks.push_back({ highest, lowest });
			MIPSample = 0;
		}
		else
		{
			MIPSample += 1;
		}
	}

	fclose(file);
}

