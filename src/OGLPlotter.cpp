#include "OGLPlotter.hpp"

namespace hpl {
	
void OGLPlotter::del(OGLLines& target)
{
	glDeleteBuffers(1, &target.lineBuffer);
}
	
void OGLPlotter::syn(Lines const& ref, OGLLines& target)
{
	float* interleave = new float[2 * ref.n()];
	for (int i = 0; i < ref.n(); i++) {
		interleave[(i << 1)] = (ref.x()[i] - ref.xmin()) / (ref.xmax() - ref.xmin());
		interleave[(i << 1) + 1] = (ref.y()[i] - ref.ymin()) / (ref.ymax() - ref.ymin());
	}

	glGenBuffers(1, &target.lineBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, target.lineBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * ref.n() * sizeof(float), interleave, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	target.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
	target.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
	target.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
	target.linemvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");
	
	target.n = ref.n();
	target.g = ref.getGeometry();
	target.c = ref.getColor();
	target.size = ref.getThickness();
	target.type = convert(ref.getDataType());

	delete[] interleave;
}

void OGLPlotter::del(OGLPoints& target)
{
	glDeleteBuffers(1, &target.pointBuffer);
}

void OGLPlotter::syn(Points const& ref, OGLPoints& target)
{
	float* interleave = new float[2 * ref.n()];
	for (int i = 0; i < ref.n(); i++) {
		interleave[(i << 1)] = (ref.x()[i] - ref.xmin()) / (ref.xmax() - ref.xmin());
		interleave[(i << 1) + 1] = (ref.y()[i] - ref.ymin()) / (ref.ymax() - ref.ymin());
	}

	glGenBuffers(1, &target.pointBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, target.pointBuffer);
	glBufferData(GL_ARRAY_BUFFER, 2 * ref.n() * sizeof(float), interleave, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	target.pos = glGetAttribLocation(programsDatabase.getLineProgram(), "Position");
	target.rect = glGetUniformLocation(programsDatabase.getLineProgram(), "Rect");
	target.color = glGetUniformLocation(programsDatabase.getLineProgram(), "Color");
	target.pointmvp = glGetUniformLocation(programsDatabase.getLineProgram(), "MVP");
		
	target.n = ref.n();
	target.g = ref.getGeometry();
	target.c = ref.getColor();
	target.size = ref.getSymbolSize();
	target.type = convert(ref.getDataType());

	delete[] interleave;
}

void OGLPlotter::del(OGLContour& target)
{
	glDeleteBuffers(1, &target.mapBuffer);
	glDeleteTextures(1, &target.textureid);
}

void OGLPlotter::syn(Contour const& ref, OGLContour& target)
{
    GLfloat quad_triangle_strip[] = {
		0.0f, 0.0f, // BL
		1.0f, 0.0f, // BR
		0.0f,  1.0f, // TL
		1.0f,  1.0f, // TR
    };

	glGenBuffers(1, &target.mapBuffer);
	glBindBuffer(GL_ARRAY_BUFFER, target.mapBuffer);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quad_triangle_strip), quad_triangle_strip, GL_STATIC_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	target.pos = glGetAttribLocation(programsDatabase.getMapProgram(), "Position");
	target.uv = glGetAttribLocation(programsDatabase.getMapProgram(), "UV");
	target.rect = glGetUniformLocation(programsDatabase.getMapProgram(), "Rect");
	target.colorMap = glGetUniformLocation(programsDatabase.getMapProgram(), "ColorMap");
	target.contourmvp = glGetUniformLocation(programsDatabase.getMapProgram(), "MVP");

	target.g = ref.getGeometry();

	glGenTextures(1, &target.textureid);

	glBindTexture(GL_TEXTURE_2D, target.textureid);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);

	float* data = new float[3 * ref.n * ref.n];
	for(int i = 0; i < ref.n*ref.n; ++i) {
		Color col = ref.getColorAtIndex(i);
		data[i*3] = col.r;
		data[i*3+1] = col.g;
		data[i*3+2] = col.b;
	}

	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB8, ref.n, ref.n, 0, GL_RGB, GL_FLOAT, data);

	delete[] data;
}

OGLPlotter::OGLPlotter() : AbstractPlotter(), Window()
{
}

OGLPlotter::~OGLPlotter()
{
}

void OGLPlotter::init()
{
    glClearColor(backgroundColor.r, backgroundColor.g, backgroundColor.b, 1.0);
    glEnable(GL_MULTISAMPLE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    //font.init(fontFile);

    programsDatabase.init();
}

void OGLPlotter::destroy()
{
    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        del(it->second);
    }

    for (auto it = pointCollection.begin(); it != pointCollection.end(); ++it) {
        del(it->second);
    }

    for (auto it = contourCollection.begin(); it != contourCollection.end(); ++it) {
        del(it->second);
    }

    programsDatabase.destroy();
}

void OGLPlotter::processDrawables()
{
	if (plots != nullptr && actualRevision != nullptr) {		
        for (auto ar = actualRevision->cbegin(); ar != actualRevision->cend(); ++ar) {
			if (revision.find(ar->first) == revision.end()) {
				revision[ar->first] = ar->second - 1;
			}
		}
		
		for (auto r = revision.begin(); r != revision.end(); ++r) {
			auto ar = actualRevision->find(r->first);
			if (ar == actualRevision->end()) {
				auto l = lineCollection.find(r->first);
				if (l != lineCollection.end()) {
					del(l->second);
					lineCollection.erase(l);
				} else {
					auto p = pointCollection.find(r->first);
					if (p != pointCollection.end()) {
						del(p->second);
						pointCollection.erase(p);
					} else {
						auto c = contourCollection.find(r->first);
						if (c != contourCollection.end()) {
							del(c->second);
							contourCollection.erase(c);
						}
					}
				}

			} else if (r->second != ar->second) {
				Drawable const* da = &plots->lookup(r->first);
				Lines const* al = dynamic_cast<Lines const*>(da);
				if (al != nullptr) {
					OGLLines& l = lineCollection[r->first];
					del(l);
					syn(*al, l);
				} else {
					Points const* ap = dynamic_cast<Points const*>(da);
					if (ap != nullptr) {
						OGLPoints& p = pointCollection[r->first];
						del(p);
						syn(*ap, p);
					} else {
						Contour const* ac = dynamic_cast<Contour const*>(da); 
						if (ac != nullptr) {
							OGLContour& c = contourCollection[r->first];
							del(c);
							syn(*ac, c);
						}
					}
				}
				r->second = ar->second;
			}
		}
	}
}

void OGLPlotter::draw()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    for (auto it = lineCollection.begin(); it != lineCollection.end(); ++it) {
        glUseProgram(programsDatabase.getLineProgram());
        glBindBuffer(GL_ARRAY_BUFFER, it->second.lineBuffer);
        glVertexAttribPointer(
            it->second.pos,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid const*) 0
        );
        glEnableVertexAttribArray(it->second.pos);
        glUniform4f(it->second.rect, it->second.g.leftOffset, it->second.g.topOffset, it->second.g.width, it->second.g.height);
        glUniform3f(it->second.color, it->second.c.r, it->second.c.g, it->second.c.b);
        glUniformMatrix3fv(it->second.linemvp, 1, GL_FALSE, mvp);

        glLineWidth(it->second.size);
        glPointSize(it->second.size);
        glDrawArrays(it->second.type, 0, it->second.n);
        glLineWidth(1.0f);
        glPointSize(1.0f);
        glDisableVertexAttribArray(it->second.pos);
    }

    for (auto it = pointCollection.begin(); it != pointCollection.end(); ++it) {
        glUseProgram(programsDatabase.getLineProgram());
        glBindBuffer(GL_ARRAY_BUFFER, it->second.pointBuffer);
        glVertexAttribPointer(
            it->second.pos,
            2,
            GL_FLOAT,
            GL_FALSE,
            0,
            (GLvoid const*) 0
        );
        glEnableVertexAttribArray(it->second.pos);
        glUniform4f(it->second.rect, it->second.g.leftOffset, it->second.g.topOffset, it->second.g.width, it->second.g.height);
        glUniform3f(it->second.color, it->second.c.r, it->second.c.g, it->second.c.b);
        glUniformMatrix3fv(it->second.pointmvp, 1, GL_FALSE, mvp);

        glPointSize(it->second.size);
        glDrawArrays(it->second.type, 0, it->second.n);
        glPointSize(1.0f);
        glDisableVertexAttribArray(it->second.pos);
    }

    for (auto it = contourCollection.begin(); it != contourCollection.end(); ++it) {
        glUseProgram(programsDatabase.getMapProgram());
        glEnableVertexAttribArray(it->second.pos);
        glEnableVertexAttribArray(it->second.uv);
        glBindBuffer(GL_ARRAY_BUFFER, it->second.mapBuffer);
        glVertexAttribPointer(
            it->second.pos,
            2,
            GL_FLOAT,
            GL_FALSE,
            2*sizeof(GLfloat),
            (GLvoid const*) 0
        );
        glVertexAttribPointer(
            it->second.uv,
            2,
            GL_FLOAT,
            GL_FALSE,
            2*sizeof(GLfloat),
            (GLvoid const*) 0
        );
        glUniform4f(it->second.rect, it->second.g.leftOffset, it->second.g.topOffset, it->second.g.width, it->second.g.height);
        glUniform1i(it->second.colorMap, 0);
        glActiveTexture(GL_TEXTURE0);
        glBindTexture(GL_TEXTURE_2D, it->second.textureid);
        glUniformMatrix3fv(it->second.contourmvp, 1, GL_FALSE, mvp);

        glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
        glDisableVertexAttribArray(it->second.pos);
        glDisableVertexAttribArray(it->second.uv);
    }
}

void OGLPlotter::resetEvent()
{
    mvp[0] = 1.0;
    mvp[4] = 1.0;
    mvp[6] = 0.0;
    mvp[7] = 0.0;
}

void OGLPlotter::moveEvent(int deltax, int deltay)
{
    mvp[6] += deltax / static_cast<double>(width);
    mvp[7] += -deltay / static_cast<double>(height);
}

void OGLPlotter::mouseWheelEvent(int x, int y, double delta)
{
    delta *= 0.2f;
    double old = mvp[0];
    mvp[0] += delta;
    mvp[4] += delta;
    if (mvp[0] < 0.5f) {
        mvp[0] = 0.5f;
    }
    if (mvp[4] < 0.5f) {
        mvp[4] = 0.5f;
    }
    double xs = (x / static_cast<double>(width) - mvp[6]) / old;
    double ys = (1.0 - y / static_cast<double>(height) - mvp[7]) / old;
    mvp[6] += (old-mvp[0]) * xs;
    mvp[7] += (old-mvp[4]) * ys;
}

GLenum OGLPlotter::convert(Drawable::Type type)
{
    switch(type) {
        case Drawable::Type_Lines:
            return GL_LINES;
        case Drawable::Type_LineStrips:
            return GL_LINE_STRIP;
        case Drawable::Type_Points:
            return GL_POINTS;
        case Drawable::Type_Texture:
            return GL_TRIANGLE_STRIP;
    }
    return 0;
}
}
