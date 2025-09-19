package com.baabeli.finalthrust;

import org.libsdl.app.SDLActivity;

public class FinalThrustActivity extends SDLActivity {
    protected String[] getLibraries() {
        return new String[] {             "SDL3",
             "SDL3_image",
             "SDL3_mixer",
            // "SDL3_net",
             "SDL3_ttf",
            "main" };
    }
}
