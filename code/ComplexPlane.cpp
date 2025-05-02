#include "ComplexPlane.h"

ComplexPlane::ComplexPlane(int pixelWidth, int pixelHeight) {
	m_pixel_size.x = pixelWidth;
	m_pixel_size.y = pixelHeight;

	m_aspectRatio = (float)pixelHeight / (float)pixelWidth;
	m_plane_center = { 0, 0 };
	m_plane_size = { BASE_WIDTH, BASE_HEIGHT * m_aspectRatio };
	m_zoomCount = 0;
	m_State = State::CALCULATING;
	m_vArray.setPrimitiveType(sf::PrimitiveType::Points);
	m_vArray.resize(pixelWidth * pixelHeight);

}

void ComplexPlane::draw(sf::RenderTarget& target, sf::RenderStates states) const {
	target.draw(m_vArray);
}

void ComplexPlane::updateRender() {
	sf::Vector2f convertedCoord;
	size_t iterations;
	sf::Uint8 r, g, b;
	if (m_State == State::CALCULATING) {

		for (int j = 0; j < m_pixel_size.x; j++) {
			for (int i = 0; i < m_pixel_size.y; i++) {
				m_vArray[j + i * m_pixel_size.x].position = { (float)j, (float)i };
				convertedCoord = mapPixelToCoords(sf::Vector2i(j, i));
				iterations = countIterations(convertedCoord);
				iterationsToRgb(iterations, r, g, b);
				m_vArray[j + i * m_pixel_size.x].color = { r, g ,b };

			}
		}
		m_State = State::DISPLAYING;
	}
}

//i dont even know bruh
sf::Vector2f ComplexPlane::mapPixelToCoords(sf::Vector2i mousePixel) {
	const int X_LEFT_BOUND = -2;
	const int X_RIGHT_BOUND = 2;

	float xConverted, yConverted;

	xConverted = ((float)mousePixel.x / (float)m_pixel_size.x) * m_plane_size.x + (m_plane_center.x - m_plane_size.x / 2.0);
	yConverted = ((float)(mousePixel.y - m_pixel_size.y) / (float)(0 - m_pixel_size.y)) * m_plane_size.y + (m_plane_center.y - m_plane_size.y / 2.0);
	return { xConverted, yConverted };
}

size_t ComplexPlane::countIterations(sf::Vector2f coord) {
	std::complex<double> c(coord.x, coord.y);
	std::complex<double> z(0, 0);
	int iterations = 0;
	while (iterations < MAX_ITER && std::abs(z) < 2) {
		z = z * z + c;
		iterations++;
	}

	return iterations;
}

void ComplexPlane::iterationsToRgb(size_t count, sf::Uint8& r, sf::Uint8& g, sf::Uint8& b) {
	//turns 0 - 64 into 0 - 255
	double iterationToColorFactor = 255.0f / (float)MAX_ITER;
	double ratio = (float)(count * iterationToColorFactor) / (float)255;
	bool override = false;

	int rColorFactor, gColorFactor, bColorFactor;

	int colorJumps = MAX_ITER / 9;

	if (count >= MAX_ITER) {
		rColorFactor = 38;
		gColorFactor = 41;
		bColorFactor = 50;
		override = true;
	}
	else if (count >= MAX_ITER - (colorJumps)) {
		rColorFactor = 112;
		gColorFactor = 0;
		bColorFactor = 0;
	}
	else if (count >= MAX_ITER - (colorJumps * 2)) {
		rColorFactor = 253;
		gColorFactor = 244;
		bColorFactor = 0;
	}
	else if (count >= MAX_ITER - (colorJumps * 3)) {
		rColorFactor = 25;
		gColorFactor = 197;
		bColorFactor = 175;
	}
	else if (count >= MAX_ITER - (colorJumps * 4)) {
		rColorFactor = 54;
		gColorFactor = 235;
		bColorFactor = 242;
	}
	else if (count >= MAX_ITER - (colorJumps * 5)) {
		rColorFactor = 148;
		gColorFactor = 112;
		bColorFactor = 220;
	}
	else if (count >= MAX_ITER - (colorJumps * 6)) {
		rColorFactor = 203;
		gColorFactor = 29;
		bColorFactor = 206;
	}
	else if (count >= MAX_ITER - (colorJumps * 7)) {
		rColorFactor = 228;
		gColorFactor = 85;
		bColorFactor = 173;
	}
	else {
		rColorFactor = 208;
		gColorFactor = 197;
		bColorFactor = 191;
	}
	//color application
	if (override) {
		r = rColorFactor;
		g = gColorFactor;
		b = bColorFactor;
	}
	else {
		r = rColorFactor * (1 - ratio);
		g = gColorFactor * (1 - ratio);
		b = bColorFactor * (1 - ratio);
	}
}

void ComplexPlane::zoomIn() {
	m_zoomCount++;
	float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);

	m_plane_size = { xSize, ySize };
	m_State = State::CALCULATING;
}

void ComplexPlane::zoomOut() {
	m_zoomCount--;
	float xSize = BASE_WIDTH * std::pow(BASE_ZOOM, m_zoomCount);
	float ySize = BASE_HEIGHT * m_aspectRatio * std::pow(BASE_ZOOM, m_zoomCount);

	m_plane_size = { xSize, ySize };
	m_State = State::CALCULATING;
}

void ComplexPlane::setCenter(sf::Vector2i mousePixel) {
	m_plane_center = mapPixelToCoords(mousePixel);
	m_State = State::CALCULATING;
}

void ComplexPlane::setMouseLocation(sf::Vector2i mousePixel) {
	m_mouseLocation = mapPixelToCoords(mousePixel);
}

void ComplexPlane::loadText(sf::Text& text) {
	std::ostringstream oss;
	oss << "Mandelbrot Set" << std::endl;
	oss << "Center : (" << m_plane_center.x << ", " << m_plane_center.y << ")" << std::endl;
	oss << "Cursor: (" << m_mouseLocation.x << ", " << m_mouseLocation.y << ")" << std::endl;
	oss << "Left-click to Zoom in" << std::endl;
	oss << "Right-click to Zoom out" << std::endl;

	text.setString(oss.str());
}
