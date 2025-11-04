#ifndef PROTECTTHING_DDOS_PROTECTION_WEB_H
#define PROTECTTHING_DDOS_PROTECTION_WEB_H

#include <string>

const std::string error_page_html = R"html(
<!DOCTYPE html>
<html lang="en">
<head>
    <meta charset="UTF-8">
    <meta name="viewport" content="width=device-width, initial-scale=1.0">
        <title>Access Temporarily Restricted</title>
        <style>
            body {
                font-family: 'SF Pro Display', -apple-system, BlinkMacSystemFont, "Segoe UI", Roboto, Helvetica, Arial, sans-serif;
                background: #000000;
                color: #e0e0e0;
                display: flex;
                justify-content: center;
                align-items: center;
                height: 100vh;
                margin: 0;
                overflow: hidden;
                position: relative;
            }
    
            .shapes-container {
                position: absolute;
                top: 0;
                left: 0;
                width: 100%;
                height: 100%;
                filter: blur(500px);
                -webkit-filter: blur(500px);
                z-index: -1;
            }
    
            .shape {
                position: absolute;
                border-radius: 50%;
                opacity: 0.4;
                animation: bounce 30s infinite ease-in-out alternate;
            }
    
            .shape1 { width: 450px; height: 450px; top: -5%; left: -5%; background: #8a2be2; animation-duration: 30s; }
            .shape2 { width: 350px; height: 350px; bottom: -5%; right: -5%; background: #9932cc; animation-duration: 25s; animation-delay: -5s; }
            .shape3 { width: 250px; height: 250px; top: 50%; left: 50%; background: #4b0082; animation-duration: 35s; animation-delay: -10s; }
            .shape4 { width: 200px; height: 200px; top: 20%; left: 80%; background: #da70d6; animation-duration: 20s; animation-delay: -2s; }
            .shape5 { width: 150px; height: 150px; top: 80%; left: 10%; background: #ba55d3; animation-duration: 40s; animation-delay: -8s; }
            .shape6 { width: 100px; height: 100px; top: 5%; left: 40%; background: #8a2be2; animation-duration: 22s; animation-delay: -15s; }
            .shape7 { width: 80px; height: 80px; top: 90%; left: 85%; background: #9932cc; animation-duration: 28s; animation-delay: -3s; }
            .shape8 { width: 120px; height: 120px; top: 30%; left: 15%; background: #4b0082; animation-duration: 32s; animation-delay: -7s; }
            .shape9 { width: 60px; height: 60px; top: 70%; left: 35%; background: #da70d6; animation-duration: 18s; animation-delay: -12s; }
            .shape10 { width: 90px; height: 90px; top: 45%; left: 75%; background: #ba55d3; animation-duration: 38s; animation-delay: -1s; }
            .shape11 { width: 70px; height: 70px; top: 10%; left: 65%; background: #8a2be2; animation-duration: 26s; animation-delay: -6s; }
            .shape12 { width: 110px; height: 110px; top: 85%; left: 55%; background: #9932cc; animation-duration: 34s; animation-delay: -11s; }
            .shape13 { width: 50px; height: 50px; top: 15%; left: 90%; background: #4b0082; animation-duration: 21s; animation-delay: -4s; }
            .shape14 { width: 130px; height: 130px; top: 60%; left: 20%; background: #da70d6; animation-duration: 29s; animation-delay: -9s; }
            .shape15 { width: 75px; height: 75px; top: 35%; left: 50%; background: #ba55d3; animation-duration: 23s; animation-delay: -14s; }
    
    
            @keyframes bounce {
                0% { transform: translate(0, 0) scale(1); }
                25% { transform: translate(80px, 120px) scale(1.1); }
                50% { transform: translate(-120px, 80px) scale(0.9); }
                75% { transform: translate(120px, -80px) scale(1.2); }
                100% { transform: translate(0, 0) scale(1); }
            }
    
            .content {
                text-align: center;
                padding: 20px;
                z-index: 1;
            }
            h1 {
                font-size: 3em;
                margin-bottom: 0.6em;
                color: #ffffff;
                font-weight: 500;
                text-shadow: 0 0 10px rgba(180, 140, 255, 0.3);
            }
            p {
                font-size: 1.2em;
                margin-bottom: 1.5em;
                color: #a0a0a0;
                max-width: 550px;
                margin-left: auto;
                margin-right: auto;
                line-height: 1.8;
                font-weight: 400;
            }
            .incident-id {
                font-size: 0.9em;
                color: #666666;
                margin-bottom: 2.5em;
            }
            hr {
                border: 0;
                height: 1px;
                background: rgba(180, 140, 255, 0.15);
                box-shadow: 0 0 8px rgba(180, 140, 255, 0.2);
                width: 80px;
                margin: 2.5em auto;
            }
            .footer {
                margin-top: 2.5em;
                font-size: 0.9em;
                color: #555555;
            }
        </style>
    </head>
    <body>
        <div class="shapes-container">
            <div class="shape shape1"></div>
            <div class="shape shape2"></div>
            <div class="shape shape3"></div>
            <div class="shape shape4"></div>
            <div class="shape shape5"></div>
            <div class="shape shape6"></div>
            <div class="shape shape7"></div>
            <div class="shape shape8"></div>
            <div class="shape shape9"></div>
            <div class="shape shape10"></div>
            <div class="shape shape11"></div>
            <div class="shape shape12"></div>
            <div class="shape shape13"></div>
            <div class="shape shape14"></div>
            <div class="shape shape15"></div>
        </div>
        <div class="content">
            <h1>Access Restricted</h1>
            <p>Our security systems have detected unusual activity from your IP address. To protect the service, we have temporarily limited access.</p>
            <hr>
            <p>Please wait a few moments before trying again. If the issue persists, please contact support and provide the following incident ID:</p>
            <p class="incident-id">5a7b9c1d-3e2f-4c8d-b1a2-9f8e7d6c5b4a</p>
            <div class="footer">
                <p>Protected by ProtectThing</p>
            </div>
        </div>
    </body>
    </html>
    )html";
#endif // PROTECTTHING_DDOS_PROTECTION_WEB_H
