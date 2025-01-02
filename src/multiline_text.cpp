#include "multiline_text.hpp"

void MultilineText::draw(sf::RenderTarget &target,
                         sf::RenderStates states) const {
    states.transform *= getTransform();
    // no texture
    states.texture = nullptr;

    for (auto line : this->lines) {
        target.draw(line);
    }
}
