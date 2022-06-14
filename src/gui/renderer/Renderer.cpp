#include "Renderer.h"

#include <algorithm>
#include <iostream>

#include <loguru/loguru.hpp>
#include <channels/channels.h>

using namespace std;
namespace fs = std::filesystem;

Renderer::Renderer(ConfigParams& params, MapInfos mapInfos) :
    m_imageConfig(params.image),
    m_windowConfig(params.window),
    m_imageFolderPath(params.image.folderPath),
    m_fontFolderPath(params.font.folderPath),
    m_mapInfos(mapInfos)
{
    if (!fs::exists(m_imageFolderPath) || !fs::is_directory(m_imageFolderPath))
    {
        stringstream ss;
        ss << "The folder \"" << m_imageFolderPath << "\" does not exists";
        throw runtime_error(ss.str());
    }
    if (!fs::exists(m_fontFolderPath) || !fs::is_directory(m_fontFolderPath))
    {
        stringstream ss;
        ss << "The folder \"" << m_fontFolderPath << "\" does not exists";
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

    // ---- Camera ----
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

    // ---- Data display ----
    // Size depend on camera one
    float dataWidth = cameraWidth;
    float dataHeight = m_windowConfig.height - cameraHeight;
    m_dataBackground.setSize(sf::Vector2f(dataWidth, dataHeight));
    // Origin on top-right (in order to place it on top right of the window)
    m_dataBackground.setOrigin(sf::Vector2f(dataWidth, 0.f));
    m_dataBackground.setPosition(m_windowConfig.width, 0.f);
    m_dataBackground.setFillColor(DATA_BACKGROUND_COLOR);
    // Load font
    auto fontFilename = (m_fontFolderPath / LATO_FONT_NAME).string();
    if(!m_dataFont.loadFromFile(fontFilename))
    {
        stringstream ss;
        ss << "Error while loading font on path " << fontFilename;
        throw runtime_error(ss.str());
    }
    m_dataText.setFont(m_dataFont);
    m_dataText.setCharacterSize(25);
    m_dataText.setFillColor(DATA_TEXT_COLOR);
    m_dataText.setPosition(m_windowConfig.width - dataWidth, 0.f);
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


// ==== RENDER METHODS ====
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
    window.draw(m_cameraSprite);
}

void Renderer::renderData(sf::RenderWindow& window)
{
    window.draw(m_dataBackground);
    stringstream ss;
    ss << "State : " << (pdsChannels::state.chars[0] == 1 ? "Armed\n" : "Disarmed\n");
    ss << "Posistion X : " << pdsChannels::localPositionNed.floats[0] << "\n";
    ss << "Posistion Y : " << pdsChannels::localPositionNed.floats[1] << "\n";
    ss << "Posistion Z : " << pdsChannels::localPositionNed.floats[2] << "\n";
    ss << "Temperature : " << pdsChannels::battery.ints32[2] << " cdegC\n";
    ss << "Battery : " << pdsChannels::battery.ints32[4] << " %\n";
    ss << "roll : " << pdsChannels::attitude.floats[0] << "\n";
    ss << "roll speed : " << pdsChannels::attitude.floats[3] << "\n";
    ss << "yaw : " << pdsChannels::attitude.floats[1] << "\n";
    ss << "yaw speed : " << pdsChannels::attitude.floats[4] << "\n";
    ss << "pitch : " << pdsChannels::attitude.floats[2] << "\n";
    ss << "pitch speed : " << pdsChannels::attitude.floats[5] << "\n";
    m_dataText.setString(ss.str());
    window.draw(m_dataText);
}


// ===== PRIVATE METHODS ====
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