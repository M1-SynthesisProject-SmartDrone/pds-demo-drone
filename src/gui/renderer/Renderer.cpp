#include "Renderer.h"

#include <algorithm>
#include <iostream>

#include <loguru/loguru.hpp>
#include <channels/channels.h>

using namespace std;
namespace fs = std::filesystem;

Renderer::Renderer(ConfigImage imageConfig, ConfigWindow windowConfig, MapInfos mapInfos) :
    m_imageConfig(imageConfig),
    m_windowConfig(windowConfig),
    m_imageFolderPath(imageConfig.folderPath),
    m_mapInfos(mapInfos)
{
    if (!fs::exists(m_imageFolderPath) || !fs::is_directory(m_imageFolderPath))
    {
        stringstream ss;
        ss << "The folder \"" << imageConfig.folderPath << "\" does not exists";
        throw runtime_error(ss.str());
    }
    // Init drone
    m_droneTexture = loadTexture(DRONE_IMAGE_NAME);
    m_droneSprite.setTexture(&m_droneTexture);
    // Size of the drone depends on the window size
    float droneWidth = m_windowConfig.width * DRONE_SCALE;
    float droneHeight = m_windowConfig.height * DRONE_SCALE;
    m_droneSprite.setSize(sf::Vector2f(droneWidth, droneHeight));
    m_droneSprite.setOrigin(sf::Vector2f(droneWidth / 2, droneHeight / 2));

    // Precalculation of any point radius
    m_pointRadius = min(m_windowConfig.width, m_windowConfig.height) * 0.025f;

    // ---- Creation of the camera texture ----
    auto nbRows = pdsChannels::imageSize.uints32[0];
    auto nbCols = pdsChannels::imageSize.uints32[1];
    m_cameraTexture.create(nbCols, nbRows);
    float cameraWidth = m_windowConfig.width * CAMERA_SCALE;
    float cameraHeight = m_windowConfig.height * CAMERA_SCALE;
    m_cameraSprite.setTexture(&m_cameraTexture);
    m_cameraSprite.setSize(sf::Vector2f(cameraWidth, cameraHeight));
    // origin on bottom-right (in order to place it on bottom right of the window)
    m_cameraSprite.setOrigin(sf::Vector2f(cameraWidth, cameraHeight));
    m_cameraSprite.setPosition(m_windowConfig.width, m_windowConfig.height);
}

Renderer::~Renderer()
{}

sf::Texture Renderer::loadTexture(string baseFilename)
{
    fs::path baseFilePath(baseFilename);
    auto fullPath = m_imageFolderPath / baseFilePath;
    sf::Texture texture;
    if (!texture.loadFromFile(fullPath.string()))
    {
        stringstream ss;
        ss << "Error while loading texture on path " << fullPath;
        throw runtime_error(ss.str());
    }
    texture.setSmooth(m_imageConfig.enableSmooth);
    return texture;
}

void Renderer::renderGrid(sf::RenderWindow& window)
{
    float midGridThickness = GRID_THICKNESS / 2.0f;

    sf::RectangleShape lineX(sf::Vector2f(m_windowConfig.width, GRID_THICKNESS));
    sf::RectangleShape lineY(sf::Vector2f(GRID_THICKNESS, m_windowConfig.height));

    lineX.setFillColor(GRID_COLOR);
    lineY.setFillColor(GRID_COLOR);

    lineY.setPosition((m_windowConfig.width / 2) - midGridThickness, 0.0f);
    lineX.setPosition(0.0f, (m_windowConfig.height / 2) - midGridThickness);
    window.draw(lineX);
    window.draw(lineY);
}

void Renderer::renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window)
{
    m_droneSprite.setPosition(calculatePos(droneCoordinates));
    m_droneSprite.setRotation(droneCoordinates.rotation);
    window.draw(m_droneSprite);
}


void Renderer::renderCameraImage(sf::RenderWindow& window)
{
    cv::Mat img(pdsChannels::imageSize.uints32[0], pdsChannels::imageSize.uints32[1], CV_8UC3, pdsChannels::image.uchars);
    cv::cvtColor(img, m_cameraMatRGBA, cv::COLOR_BGR2RGBA);
    // We must update the texture data
    m_cameraImage.create(m_cameraMatRGBA.cols, m_cameraMatRGBA.rows, m_cameraMatRGBA.ptr());
    m_cameraTexture.loadFromImage(m_cameraImage);
    m_cameraSprite.setTexture(&m_cameraTexture);
    window.draw(m_cameraSprite);
}

float Renderer::calculateXPos(float x)
{
    auto& m = m_mapInfos;
    float distRatio = (x - m.minX) / m.width;
    return (distRatio * m_windowConfig.width);
}

float Renderer::calculateYPos(float y)
{
    auto& m = m_mapInfos;
    float distRatio = (y - m.minY) / m.height;
    return (distRatio * m_windowConfig.height);
}

sf::Vector2f Renderer::calculatePos(const Coordinates& coordinates)
{
    return {
        calculateXPos(coordinates.x),
        calculateYPos(coordinates.y)
    };
}

float Renderer::calculateRadius(float r)
{
    auto& m = m_mapInfos;
    return (r / m.width) * m_windowConfig.width;
}

sf::CircleShape Renderer::createCircle(float radius, const sf::Color& color, bool colorOutline)
{
    float displayedRadius = calculateRadius(radius);
    sf::CircleShape circle(displayedRadius);
    if (colorOutline)
    {
        circle.setFillColor(sf::Color::Transparent);
        circle.setOutlineColor(color);
        circle.setOutlineThickness(1.0f);
    }
    else
    {
        circle.setFillColor(color);
    }
    circle.setOrigin(displayedRadius, displayedRadius);
    return circle;
}