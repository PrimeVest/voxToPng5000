
#include "node_shape.hpp"
#include "loader.hpp"
#include "scene.hpp"
#include "model.hpp"

NodeShapeModel::NodeShapeModel(const Context& context)
{
	context.pScene->registerShape(this);
};

int32_t NodeShapeModel::getId() const
{
	return m_id;
}

error NodeShapeModel::load(Loader& loader)
{
	m_id = loader.readNextInt32();
	error err = m_attributes.load(loader);
	if(err != ""s)
	{
		return err;
	}
	return ""s;
}

const weak_ptr<Model>& NodeShapeModel::getModel() const
{
	return m_pModel;
}

void NodeShapeModel::setModel(const weak_ptr<Model>& pModel)
{
	m_pModel = pModel;
}

NodeShape::NodeShape(const Context& context)
	: Node(context)
{

}

error NodeShape::load(Loader& loader)
{
	error err = Node::load(loader);
	if(err != ""s)
	{
		return err;
	}

	int32_t numModels = loader.readNextInt32();
	models.reserve((size_t)numModels);
	for(size_t i=0; i<numModels; i++)
	{
		shared_ptr<NodeShapeModel> model(new NodeShapeModel(getContext()));
		err = model->load(loader);
		if(err != ""s)
		{
			return err;
		}
		models.push_back(model);
	}

	return ""s;
}

const Color* NodeShape::getVoxel(int x, int y, int z) const
{
	for(auto&& modelWeak : models)
	{
		shared_ptr<Model> model = modelWeak->getModel().lock();
		const vector<Voxel>& voxels = model->getVoxels();

		for(auto&& voxel : voxels)
		{
			if(voxel.x == x && voxel.y == y && voxel.z == z)
			{
				return &getContext().pScene->lookupPaletteColor(voxel.colorIndex);
			}
		}
	}
	return nullptr;
}

void NodeShape::getSize(int32_t* pX, int32_t* pY, int32_t* pZ) const
{
	int32_t width = 0;
	int32_t height = 0;
	int32_t depth = 0;
	for(auto&& model : models)
	{
		int32_t modelWidth = 0;
		int32_t modelHeight = 0;
		int32_t modelDepth = 0;
		model->getModel().lock()->getSize(&modelWidth, &modelHeight, &modelDepth);

		width = max(width, modelWidth);
		height = max(height, modelHeight);
		depth = max(depth, modelDepth);
	}
	if(pX != nullptr)
	{
		*pX = width;
	}
	if(pY != nullptr)
	{
		*pY = height;
	}
	if(pZ != nullptr)
	{
		*pZ = depth;
	}
}