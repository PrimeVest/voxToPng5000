
#include "model.hpp"
#include "loader.hpp"

Model::Model(int32_t id)
	: m_id(id)
{
};

error Model::loadSize(Loader& loader)
{
	__attribute__((unused)) int32_t chunkSize = loader.readNextInt32();
	__attribute__((unused)) int32_t numChildrenChunks = loader.readNextInt32();

	m_width = loader.readNextInt32();
	m_height = loader.readNextInt32();
	m_depth = loader.readNextInt32();

	return ""s;
}

error Model::loadData(Loader& loader)
{
	__attribute__((unused)) int32_t chunkSize = loader.readNextInt32();
	__attribute__((unused)) int32_t numChildrenChunks = loader.readNextInt32();

	m_voxels.resize((size_t)loader.readNextInt32());
	for(auto&& v : m_voxels) {
		v.x = loader.readNextChar();
		v.y = loader.readNextChar();
		v.z = loader.readNextChar();
		v.colorIndex = (uint8_t)loader.readNextChar();
		v.colorIndex--;
	}
	return ""s;
}

int32_t	Model::getId() const
{
	return m_id;
}

void Model::getSize(int32_t* pX, int32_t* pY, int32_t* pZ) const
{
	if(pX != nullptr)
	{
		*pX = m_width;
	}
	if(pY != nullptr)
	{
		*pY = m_height;
	}
	if(pZ != nullptr)
	{
		*pZ = m_depth;
	}
}

void Model::doubleSize()
{
	m_width*=2;
	m_height*=2;
	m_depth*=2;
	vector<Voxel> oldVoxels = m_voxels;
	m_voxels.clear();
	m_voxels.reserve(oldVoxels.size()*8);

	for(auto&& oldVoxel : oldVoxels)
	{
		for(int16_t x=0; x<2; x++) for(int16_t y=0; y<2; y++) for(int16_t z=0; z<2; z++)
		{
			Voxel voxel = oldVoxel;
			voxel.x = (voxel.x * int16_t(2)) + x;
			voxel.y = (voxel.y * int16_t(2)) + y;
			voxel.z = (voxel.z * int16_t(2)) + z;
			m_voxels.push_back(voxel);
		}
	}
}

void Model::recenterOrigins()
{
	for (auto&& v : m_voxels)
	{
		v.x -= (m_width/2);
		v.y -= (m_height/2);
		v.z -= (m_depth/2);
	}

	/*
	int16_t x0 = INT16_MAX;
	int16_t y0 = INT16_MAX;
	int16_t z0 = INT16_MAX;
	int16_t x1 = -INT16_MAX;
	int16_t y1 = -INT16_MAX;
	int16_t z1 = -INT16_MAX;
	for (auto&& v : m_voxels)
	{
		x0 = min(v.x, x0);
		x1 = max(v.x, x1);
		y0 = min(v.y, y0);
		y1 = max(v.y, y1);
		z0 = min(v.z, z0);
		z1 = max(v.z, z1);
	}
	int16_t centerPosOffsX = ((x1-x0)+1)/2;
	int16_t centerPosOffsY = ((y1-y0)+1)/2;
	int16_t centerPosOffsZ = ((z1-z0)+1)/2;

	for (auto&& v : m_voxels)
	{
		v.x -= centerPosOffsX;
		v.y -= centerPosOffsY;
		v.z -= centerPosOffsZ;
	}
	 */
}

const vector<Voxel>& Model::getVoxels() const
{
	return m_voxels;
}



