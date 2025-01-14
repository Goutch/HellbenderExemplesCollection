#include "HBE.h"

using namespace HBE;

class CubeScene : public Scene {
	Mesh *cube_mesh;
	Shader *fragment_shader;
	Shader *vertex_shader;
	RasterizationPipeline *pipeline;
	RasterizationPipelineInstance *pipeline_instance;
public:

	CubeScene() {
		createResources();
		setupScene();
	}

	~CubeScene() {
		delete cube_mesh;
		delete fragment_shader;
		delete vertex_shader;
		delete pipeline;
		delete pipeline_instance;
	}

	void createResources() {
		ShaderInfo shader_info{};
		shader_info.path = "shaders/defaults/Position.frag";
		shader_info.stage = SHADER_STAGE_FRAGMENT;
		fragment_shader = Resources::createShader(shader_info);

		shader_info.path = "shaders/defaults/Position.vert";
		shader_info.stage = SHADER_STAGE_VERTEX;
		vertex_shader = Resources::createShader(shader_info);

		RasterizationPipelineInfo pipeline_info{};
		pipeline_info.attribute_info_count = 1;
		pipeline_info.attribute_infos = &VERTEX_ATTRIBUTE_INFO_POSITION3D;
		pipeline_info.flags = RASTERIZATION_PIPELINE_FLAG_CULL_BACK;
		pipeline_info.fragment_shader = fragment_shader;
		pipeline_info.vertex_shader = vertex_shader;

		pipeline = Resources::createRasterizationPipeline(pipeline_info);

		RasterizationPipelineInstanceInfo pipeline_instance_info{};
		pipeline_instance_info.rasterization_pipeline = pipeline;
		pipeline_instance = Resources::createRasterizationPipelineInstance(pipeline_instance_info);

		MeshInfo mesh_info{};
		mesh_info.attribute_infos = &VERTEX_ATTRIBUTE_INFO_POSITION3D;
		mesh_info.attribute_info_count = 1;

		cube_mesh = Resources::createMesh(mesh_info);
		Geometry::createCube(*cube_mesh, 1, 1, 1, VERTEX_FLAG_NONE);
	}

	void createCubeMesh() {
		Entity cube_entity = createEntity3D();
		MeshRenderer *cube_renderer = cube_entity.attach<MeshRenderer>();
		cube_renderer->mesh = cube_mesh;
		cube_renderer->pipeline_instance = pipeline_instance;
		cube_entity.get<Transform>()->translate(vec3(0, 0, -5));
	}

	void setupScene() {
		createCubeMesh();

		Entity camera_entity = createEntity3D();
		camera_entity.attach<Camera>();

		vec4 c = vec4(1, 0, 0, 1);
		pipeline_instance->setUniform("material", &c);
	}

};