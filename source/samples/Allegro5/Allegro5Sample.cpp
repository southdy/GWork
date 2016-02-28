/*
 *  Gwork
 *  Copyright (c) 2012 Facepunch Studios
 *  Copyright (c) 2013-2016 Billy Quith
 *  See license in Gwork.h
 */

#include <Gwork/Gwork.h>
#include <Gwork/Skins/Simple.h>
#include <Gwork/Skins/TexturedBase.h>
#include <Gwork/Test/Test.h>
#include <Gwork/Input/Allegro.h>
#include <Gwork/Renderers/Allegro.h>

#include <allegro5/allegro.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_primitives.h>


int main(int argc, char** argv)
{
    if (!al_init())
        return -1;

    ALLEGRO_DISPLAY* display = al_create_display(1024, 768);

    if (!display)
        return -1;

    ALLEGRO_EVENT_QUEUE* event_queue = al_create_event_queue();

    if (!event_queue)
        return -1;

    al_init_image_addon();
    al_init_font_addon();
    al_init_primitives_addon();
    al_init_ttf_addon();
    al_install_mouse();
    al_install_keyboard();
    al_register_event_source(event_queue, al_get_display_event_source(display));
    al_register_event_source(event_queue, al_get_mouse_event_source());
    al_register_event_source(event_queue, al_get_keyboard_event_source());
    
    // Create a Gwork Allegro Renderer
    Gwk::Renderer::Allegro* pRenderer = new Gwk::Renderer::Allegro();

    // Create a Gwork skin
    Gwk::Skin::TexturedBase skin(pRenderer);
    skin.SetRender(pRenderer);
    skin.Init("DefaultSkin.png");
    
    // The fonts work differently in Allegro - it can't use
    // system fonts. So force the skin to use a local one.
    // Note, you can get fonts that cover many languages/locales to do Chinese,
    //       Arabic, Korean, etc. e.g. "Arial Unicode" (but it's 23MB!).
    skin.SetDefaultFont("OpenSans.ttf", 11);
    
    // Create a Canvas (it's root, on which all other Gwork panels are created)
    Gwk::Controls::Canvas* pCanvas = new Gwk::Controls::Canvas(&skin);
    pCanvas->SetSize(1024, 768);
    pCanvas->SetDrawBackground(true);
    pCanvas->SetBackgroundColor(Gwk::Color(150, 170, 170, 255));

    // Create our unittest control (which is a Window with controls in it)
    UnitTest* pUnit = new UnitTest(pCanvas);
    pUnit->SetPos(10, 10);

    // Create a Windows Control helper
    // (Processes Windows MSG's and fires input at Gwork)
    Gwk::Input::Allegro GworkInput;
    GworkInput.Initialize(pCanvas);
    ALLEGRO_EVENT ev;
    bool bQuit = false;

    while (!bQuit)
    {
        while (al_get_next_event(event_queue, &ev))
        {
            if (ev.type == ALLEGRO_EVENT_DISPLAY_CLOSE)
                bQuit = true;

            GworkInput.ProcessMessage(ev);
        }

        pCanvas->RenderCanvas();
        al_flip_display();
        
        al_rest(0.001);
    }

    al_destroy_display(display);
    al_destroy_event_queue(event_queue);
    return 0;
}