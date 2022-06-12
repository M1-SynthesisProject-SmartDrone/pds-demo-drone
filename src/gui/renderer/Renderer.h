#ifndef __RENDERER_H__
#define __RENDERER_H__

#include <vector>
#include <filesystem>

#include <SFML/Graphics.hpp>
#include <opencv2/opencv.hpp>

#include "config/ConfigParams.h"
#include "engine/Coordinates.h"
#include "engine/MapInfos.h"

class Renderer
{
private:
    // ==== IMAGE FILENAMES ====
    static inline const std::string DRONE_IMAGE_NAME = "drone.png";
    
    // ==== FONT FILENAMES ====
    static inline const std::string LATO_FONT_NAME = "Lato-Regular.ttf";

    // ==== COLORS ====
    static inline const sf::Color GRID_COLOR = sf::Color(105, 105, 105);
    static inline const sf::Color DATA_BACKGROUND_COLOR = sf::Color::Black;
    static inline const sf::Color DATA_TEXT_COLOR = sf::Color::White;

    // ==== SIZES ====
    static inline const float GRID_THICKNESS = 2.0f;

    const MapInfos m_mapInfos;
    const ConfigWindow m_windowConfig;
    const ConfigImage m_imageConfig;
    const ConfigFont m_fontConfig;
    const std::filesystem::path m_imageFolderPath;
    const std::filesystem::path m_fontFolderPath;

    // This is kept as we load an image for this element
    static inline constexpr float DRONE_SCALE = 0.07f;
    sf::Texture m_droneTexture;
    sf::RectangleShape m_droneSprite;

    // We don't want to calculate some values over and over
    float m_pointRadius;

    // Camera texture
    static inline const float CAMERA_SCALE = 0.3f;
    cv::Mat m_cameraMatRGBA;
    sf::Image m_cameraImage;
    sf::Texture m_cameraTexture;
    sf::RectangleShape m_cameraSprite;

    // Data display
    sf::RectangleShape m_dataBackground;
    sf::Font m_dataFont;
    sf::Text m_dataText;

    // ==== PRIVATE METHODS ====
    /**
     * Load a texture from the base filename wanted (do not put the folder name)
     */
    sf::Texture loadTexture(std::string baseFilename);

    // ---- Position & size calculation from the map & window infos ----
    float calculateXPos(float x);
    float calculateYPos(float y);
    sf::Vector2f calculatePos(const Coordinates& coordinates);
    float calculateRadius(float r);

    // ---- Shape creation ----
    /**
     * Create a circle with the origin at the center.
     * If "colorOutline" is set to false, it will create a disk (with color filling all).
     * Otherwise, a circle with only the outline will be created 
     * (with the fill color set to Transparent).
     */
    sf::CircleShape createCircle(float radius, const sf::Color& color, bool colorOutline = false);

    // ---- Camera settings ----
    // void fillRgbaData(unsigned char* rgbData, uint32_t length);

public:
    /**
     * This will check if the config is valid (correct image folder path, etc.)
     */ 
    Renderer(ConfigParams& params, MapInfos mapInfos);
    ~Renderer();

    /**
     * Draw a grid in on the window (for debugging puposes)
     */
    void renderGrid(sf::RenderWindow& window);
    void renderDrone(Coordinates& droneCoordinates, sf::RenderWindow& window);
    void renderCameraImage(sf::RenderWindow& window);
    void renderData(sf::RenderWindow& window);
};

#endif // __RENDERER_H__