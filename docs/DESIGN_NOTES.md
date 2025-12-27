# Design Notes

## Logo Analysis (`bobsgame.png`)

### Current Defects
1.  **Contrast Issues:** The pure white text ("bob's game") has low visibility against the brighter parts of the gradient background (Cyan/Green). This impacts readability.
2.  **Style Inconsistency:** The logo mixes two distinct aesthetic styles:
    *   **Text:** Sharp, pixel-art font (Retro/8-bit).
    *   **Background:** Smooth, blurry gradient (Modern/High-res).
    This mismatch creates a "muddy" look rather than a cohesive retro style.
3.  **Transparency:** The background gradient fades to alpha transparency. While technically correct for overlays, the semi-transparent edges can look "dirty" on certain backgrounds if not composited correctly.

### Recommendations
1.  **Improve Contrast:** Add a black outline (stroke) or a drop shadow to the white text to separate it from the background.
2.  **Unify Style:** Dither the background gradient. Using a dithering pattern (like Bayer or ordered dithering) would make the background look like it belongs in the same 8-bit/16-bit era as the font.
3.  **Format:** Continue using PNG for the master asset. Avoid JPG for UI elements to prevent compression artifacts.

## Visualizer & Emulation Integration
*   The ProjectM visualizer should default to presets that match the game's neon/retro aesthetic.
*   The Libretro frontend overlay should use the same pixel-art font as the main game for consistency.
