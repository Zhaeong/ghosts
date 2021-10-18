#include "game.h"

int StartSDL(SDL_Window **window, SDL_Renderer **renderer)
{

    if (SDL_Init(SDL_INIT_VIDEO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Init Audio
    if (SDL_Init(SDL_INIT_AUDIO) < 0)
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't initialize SDL: %s",
                SDL_GetError());
    }

    //Uint32 windowType = SDL_WINDOW_FULLSCREEN;
    Uint32 windowType = SDL_WINDOW_RESIZABLE;

    //SDL_WINDOW_FULLSCREEN
    //SDL_WINDOW_RESIZABLE
    if (SDL_CreateWindowAndRenderer(GAMEWIDTH,
                GAMEHEIGHT,
                windowType,
                window,
                renderer))
    {
        SDL_LogError(SDL_LOG_CATEGORY_APPLICATION,
                "Couldn't create window and renderer: %s",
                SDL_GetError());
    }

    return 0;
}

SDL_Texture *GetSDLTexture(SDL_Renderer *renderer, SDL_Window *window, string textureLocation)
{
    //Make sure to initialize texture to null or else SDL_DestroyTexture will crash program
    SDL_Texture *texture = NULL;

    SDL_Surface *loadedSurface = IMG_Load(textureLocation.c_str());

    if (loadedSurface == NULL)
    {
        printf("Unable to load image %s! SDL_image Error: %s\n",
                textureLocation.c_str(),
                IMG_GetError());
    }
    else
    {
        //Convert surface to display format

        //Note have to use SDL_PIXELFORMAT_ARGB8888 due to window using SDL_PIXELFORMAT_RGB888
        //Which doesn't have alpha channel

        //SDL_Surface* formattedSurface = SDL_ConvertSurfaceFormat( loadedSurface,
        //                                                          SDL_GetWindowPixelFormat( window ),
        //                                                          0 );

        SDL_Surface *formattedSurface = SDL_ConvertSurfaceFormat(loadedSurface,
                TEXTUREFORMAT,
                0);

        if (formattedSurface == NULL)
        {
            printf("Unable to convert loaded surface to display format! SDL Error: %s\n",
                    SDL_GetError());
        }
        else
        {
            //Create blank streamable texture

            //texture = SDL_CreateTexture( renderer,
            //                             SDL_GetWindowPixelFormat( window ),
            //                             SDL_TEXTUREACCESS_STREAMING,
            //                             formattedSurface->w,
            //                             formattedSurface->h );

            texture = SDL_CreateTexture(renderer,
                    TEXTUREFORMAT,
                    SDL_TEXTUREACCESS_STREAMING,
                    formattedSurface->w,
                    formattedSurface->h);

            //texture = SDL_CreateTextureFromSurface( renderer, formattedSurface );
            if (texture == NULL)
            {
                printf("Unable to create blank texture! SDL Error: %s\n",
                        SDL_GetError());
            }
            else
            {
                void *mPixels;
                int mPitch;

                SDL_SetTextureBlendMode(texture, SDL_BLENDMODE_BLEND);

                //Lock texture for manipulation
                if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
                {
                    printf("Unable to lock texture! %s\n", SDL_GetError());
                }

                //Copy loaded/formatted surface pixels
                memcpy(mPixels, formattedSurface->pixels, formattedSurface->pitch * formattedSurface->h);

                //Unlock texture to update
                SDL_UnlockTexture(texture);

                mPixels = NULL;

                //Get image dimensions
                //mWidth = formattedSurface->w;
                //mHeight = formattedSurface->h;
            }

            //Get rid of old formatted surface
            SDL_FreeSurface(formattedSurface);
        }

        //Get rid of old loaded surface
        SDL_FreeSurface(loadedSurface);
    }
    return texture;
}

void RemoveTextureWhiteSpace(SDL_Texture *texture)
{
    void *mPixels;
    int mPitch;

    AddUIEntity(11,11);

    if (texture == NULL)
    {
        printf("Input Texture is null in RemoveTextureWhiteSpace! SDL Error: %s\n",
                SDL_GetError());
    }

    if (SDL_LockTexture(texture, NULL, &mPixels, &mPitch) != 0)
    {
        printf("Unable to lock texture! %s\n", SDL_GetError());
    }
    else
    {
        //Allocate format from window
        //Uint32 format = SDL_GetWindowPixelFormat( window );

        SDL_PixelFormat *mappingFormat = SDL_AllocFormat(TEXTUREFORMAT);

        int texWidth = 0;
        int texHeight = 0;

        Uint32 texFormat;
        SDL_QueryTexture(texture, &texFormat, NULL, &texWidth, &texHeight);

        //Get pixel data
        Uint32 *pixels = (Uint32 *)mPixels;
        int pixelCount = (mPitch / 4) * texHeight;

        //cout << "texformat:" << texFormat << "\n";
        //cout << "texWidth:" << texWidth << "\n";
        //cout << "texHeight:" << texHeight << "\n";
        //cout << "mPitch:" << mPitch << "\n";
        //cout << "pixelCount:" << pixelCount << "\n";

        //Map colors

        Uint32 colorKeyWhite = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0xFF);
        Uint32 colorKeyGray = SDL_MapRGBA(mappingFormat, 153, 153, 153, 0xFF);

        Uint32 transparent = SDL_MapRGBA(mappingFormat, 0xFF, 0xFF, 0xFF, 0);

        //Color key pixels
        for (int i = 0; i < pixelCount; ++i)
        {
            //cout << "col:" << pixels[i] << "\n";
            //cout << "key:" << colorKey << "\n";

            if (pixels[i] == colorKeyWhite || pixels[i] == colorKeyGray)
            {
                pixels[i] = transparent;
            }
        }

        SDL_UnlockTexture(texture);
        //mPixels = NULL;
        //Free format
        SDL_FreeFormat(mappingFormat);
    }
}


AudioClip InitAudio(string filepath)
{
    AudioClip clip;
    clip.mActive = true;
    clip.wavPath = filepath;
    if (SDL_LoadWAV(clip.wavPath.c_str(), &clip.wavSpec, &clip.wavBuffer, &clip.wavLength) == NULL) 
    {
        fprintf(stderr, "Could not open wav %s: %s\n", filepath.c_str(), SDL_GetError());
    } 
    return clip;
}

void PlayAudio(GameState *GS, AudioClip clip)
{

    //using malloc and memcpy because we don't want to touch the 
    //original audio
    Uint8 *clipBuffer = (Uint8 *) malloc(clip.wavLength);
    memcpy(clipBuffer, clip.wavBuffer, clip.wavLength);

    int success = -1;

    cout << "playerAudio: " << clip.wavPath << "\n";

    //Layering sounds 
    if(GS->curSound.mActive)
    {
        //Buffer which stores the remaining data in queue 
        Uint8 *wavBuffer;

        //need to account for currently played audio 
        Uint32 sizeCurAudio = SDL_GetQueuedAudioSize(GS->audioDevice);

        wavBuffer = (Uint8 *)malloc(sizeCurAudio);

        //printf("ori:  %p\n", GS->curSound.wavBuffer);

        //This new pointer is the offsetted pointer location of the current sound that's already been played
        //We can simply clear queue and queue this pointer and it will seem like nothing happened and 
        //The track will play still as normal, but we want to mix the remaining portion with the new audio
        Uint8 *curAudioOffset = GS->curSound.wavBuffer + (GS->curSound.wavLength - sizeCurAudio);

        //printf("new:  %p\n", curAudioOffset);

        memcpy(wavBuffer, curAudioOffset, sizeCurAudio);

        //Two conditions, if the new audio that wants to be played is shorter than the remaining audio left 
        //to be played in queue, then mix the new audio with the remaining audio in queue
        if(sizeCurAudio > clip.wavLength)
        {
            SDL_MixAudioFormat(wavBuffer, clipBuffer, AUDIO_FORMAT, clip.wavLength, SDL_MIX_MAXVOLUME);

            SDL_ClearQueuedAudio(GS->audioDevice);
            success = SDL_QueueAudio(GS->audioDevice, wavBuffer, sizeCurAudio);
        }
        //else if the new audio is greater in length than the remaining audio in queue
        //then the new audio becomes mainaudio and mix the remaining audio with the new audio
        else
        {
            //there
            GS->curSound = clip;
            SDL_MixAudioFormat(clipBuffer, wavBuffer, AUDIO_FORMAT, sizeCurAudio, SDL_MIX_MAXVOLUME);
            SDL_ClearQueuedAudio(GS->audioDevice);
            success = SDL_QueueAudio(GS->audioDevice, clipBuffer, clip.wavLength);
        }

        free(wavBuffer);


    }
    else
    {
        GS->curSound = clip;
        success = SDL_QueueAudio(GS->audioDevice, clipBuffer, clip.wavLength);
    }

    free(clipBuffer);
    if(success < 0)
    {
        printf("SDL_QueueAudio failed %s, err: %s", clip.wavPath.c_str(), SDL_GetError()); 
    }

}

void PlayAudioFromPath(GameState *GS, string clipPath)
{
    for(int i = 0; i < NUM_AUDIO_CLIPS; i++)
    {
        if(clipPath == GS->acArray[i].wavPath)
        {
            PlayAudio(GS, GS->acArray[i]);
        }
    }
}
