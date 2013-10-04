#include "testsuite/testsuite.h"

#include "graphics/imageloader.h"
#include "graphics/devilwrapper.h"
#include "graphics/textureloader.h"
#include "graphics/shader.h"
#include "graphics/shaderloader.h"
#include "graphics/vert.h"
#include "graphics/model.h"
#include "graphics/modelloader.h"

/*
 * Not indenting, as it adds next to no information. The namespace is to
 * prevent testing code from polluting the rest of the project
 */
namespace UnitTesting {

#define METHOD_FAIL false

BEGIN_TEST_DEF(DevILWrapperTest) {
	//ImageLoader& obj = DevILWrapper("test.jpg");
	DevILWrapper obj("test.jpg");
	ASSERT_NEQ(0u, obj.getWidth());
	ASSERT_NEQ(0u, obj.getHeight());
	ASSERT_NEQ(static_cast<void*>(nullptr), obj.getDataPointer());
	return TEST_SUCCESS;
}
END_TEST_DEF(DevILWrapperTest);

class fakeIL : public ImageLoader {
public:
	fakeIL(){}
	virtual unsigned int getWidth() const override { return 2; }
	virtual unsigned int getHeight() const override { return 2; }
	virtual void* getDataPointer() {
		static unsigned char img[] = {
			255, 255, 255, 255,   0,   0,   0, 255,
			0,   0,   0, 255, 255, 255, 255, 255
		};
		return (void*)img;
	}
	virtual ~fakeIL(){}
};

BEGIN_TEST_DEF(TextureLoaderTest) {
	fakeIL il;
	Texture* tex = TextureLoader::LoadTexture(fakeIL(),
		TextureType::Diffuse
	);
	ASSERT_EQ(tex->getWidth(), il.getWidth());
	ASSERT_EQ(tex->getHeight(), il.getHeight());
	tex->cleanup();
	delete tex;
	return TEST_SUCCESS;
}
END_TEST_DEF(TextureLoaderTest);

std::string modBuf(
R"(v 0.0 0.0 0.0
v 1.0 0.0 0.0
v 0.0 1.0 0.0
v 1.0 1.0 0.0

f 0 1 2
f 2 1 3
)"
);
BEGIN_TEST_DEF(ModelTest) {
	Model* m = ModelLoader::LoadModelFromBuffer(modBuf);

	ASSERT_NEQ(static_cast<Model*>(nullptr), m);
	ASSERT_NEQ(0u, m->getID());
	ASSERT_NEQ(METHOD_FAIL, m->draw());
	ASSERT_NEQ(METHOD_FAIL, m->bind());
	ASSERT_NEQ(METHOD_FAIL, m->unbind());
	ASSERT_NEQ(METHOD_FAIL, m->cleanup());
	delete m;

	return TEST_SUCCESS;
}
END_TEST_DEF(ModelTest);

BEGIN_TEST_DEF(ShaderTest) {
	// Binding a shader requires that a model to bind it to :(
	Model* m = ModelLoader::LoadModelFromBuffer(modBuf);
	/*
	 * This is the first test I've attempted to write before the code it
	 * will test has been written. Forgive me :)
	 */
	std::vector<Shader*> v;
	std::string frag(
R"(#version 150
in vec4 cfv;
in vec2 tfv;
uniform sampler2D diffuseTex;

out vec4 oc;
void main() {
	oc = cfv*texture(diffuseTex,tfv);
})"
	);
	std::string vert(
R"(#version 150
in vec4 position;
in vec4 colour;
in vec2 texture;

out vec4 cfv;
out vec2 tfv;
void main() {
	gl_Position = position;
	cfv = colour;
	tfv = texture;
})"
	);
	v.push_back(ShaderLoader::LoadShaderFromBuffers(frag,vert));

	for(auto s: v) {
		ASSERT_NEQ(static_cast<decltype(s)>(nullptr), s);
		ASSERT_LT (0u, s->getShaderID());
		ASSERT_NEQ(METHOD_FAIL, s->isShaderValid());
		for(auto a: {
				VertComponent::Position,
				VertComponent::Colour,
				VertComponent::Texture
			}
		) {
			ASSERT_NEQ(
				METHOD_FAIL,
			       	s->isAttributeSupported(a)
			);
		}
		ASSERT_NEQ(METHOD_FAIL, s->set());
		ASSERT_NEQ(METHOD_FAIL, m->useShader(s));
		for(auto t: {
				TextureType::Diffuse
			}
		) {
			ASSERT_NEQ(
				METHOD_FAIL,
				s->isTextureTypeSupported(t)
			);
		}
	}
	m->cleanup();
	delete m;
	return TEST_SUCCESS;
}
END_TEST_DEF(ShaderTest);

bool RunTests() {
	return Testing::RunAllTests();
}

#undef METHOD_FAIL

} //end namespace UnitTesting
