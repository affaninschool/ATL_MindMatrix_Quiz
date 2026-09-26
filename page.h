/* ============================================================
   page.h — MindMatrix quiz web page (HTML + CSS + JS)
   Stored in PROGMEM so no LittleFS upload is required.
   ============================================================ */

#ifndef PAGE_H
#define PAGE_H

const char PAGE_HTML[] PROGMEM = R"RAWHTML(
<!DOCTYPE html>
<html lang="en">
<head>
<meta charset="UTF-8" />
<meta name="viewport" content="width=device-width, initial-scale=1.0, viewport-fit=cover" />
<meta name="theme-color" content="#06060f" />
<title>MindMatrix · Quiz</title>
<style>
  *, *::before, *::after { box-sizing: border-box; margin: 0; padding: 0; }

  :root {
    --bg-0:      #05050c;
    --bg-1:      #0a0a1a;
    --bg-2:      #12102a;
    --accent:    #00e0ff;
    --accent-2:  #b16cff;
    --accent-3:  #ff4dd2;
    --good:      #19f7a5;
    --bad:       #ff4560;
    --gold:      #ffcc4d;
    --text:      #f5f6ff;
    --muted:     #a3a4c4;
    --faint:     #6c6d92;
    --glass:     rgba(255, 255, 255, 0.045);
    --glass-2:   rgba(255, 255, 255, 0.07);
    --border:    rgba(255, 255, 255, 0.09);
    --border-2:  rgba(255, 255, 255, 0.16);
    --radius-lg: 24px;
    --radius-md: 16px;
    --radius-sm: 12px;
    --shadow-lg: 0 30px 60px -20px rgba(0,0,0,0.7),
                 0 15px 30px -15px rgba(0,0,0,0.5);
    --font-sans: 'SF Pro Display', -apple-system, BlinkMacSystemFont,
                 'Segoe UI Variable Display', 'Segoe UI', Roboto,
                 'Helvetica Neue', Arial, sans-serif;
    --font-mono: 'SF Mono', 'JetBrains Mono', 'Fira Code', ui-monospace,
                 'Cascadia Mono', 'Courier New', monospace;
  }

  html, body {
    height: 100%;
    font-family: var(--font-sans);
    color: var(--text);
    background: var(--bg-0);
    overflow-x: hidden;
    -webkit-font-smoothing: antialiased;
    -moz-osx-font-smoothing: grayscale;
    text-rendering: optimizeLegibility;
  }

  body { position: relative; min-height: 100vh; min-height: 100dvh; }

  .bg {
    position: fixed;
    inset: 0;
    z-index: 0;
    overflow: hidden;
    pointer-events: none;
    background:
      radial-gradient(ellipse 80% 60% at 50% -10%, #1a1240 0%, transparent 60%),
      radial-gradient(ellipse 60% 50% at 100% 110%, #0b1a3a 0%, transparent 60%),
      linear-gradient(180deg, var(--bg-0) 0%, var(--bg-1) 50%, var(--bg-2) 100%);
  }

  .orb {
    position: absolute;
    border-radius: 50%;
    filter: blur(90px);
    opacity: 0.55;
    will-change: transform;
  }
  .orb-1 {
    width: 480px; height: 480px;
    background: radial-gradient(circle, var(--accent) 0%, transparent 70%);
    top: -180px; left: -140px;
    animation: drift1 22s ease-in-out infinite;
  }
  .orb-2 {
    width: 520px; height: 520px;
    background: radial-gradient(circle, var(--accent-2) 0%, transparent 70%);
    bottom: -200px; right: -180px;
    animation: drift2 26s ease-in-out infinite;
  }
  .orb-3 {
    width: 380px; height: 380px;
    background: radial-gradient(circle, var(--accent-3) 0%, transparent 70%);
    top: 40%; left: 55%;
    opacity: 0.28;
    animation: drift3 30s ease-in-out infinite;
  }
  @keyframes drift1 {
    0%,100% { transform: translate(0,0) scale(1); }
    50%     { transform: translate(60px, 80px) scale(1.1); }
  }
  @keyframes drift2 {
    0%,100% { transform: translate(0,0) scale(1); }
    50%     { transform: translate(-70px, -60px) scale(1.08); }
  }
  @keyframes drift3 {
    0%,100% { transform: translate(0,0) scale(1); }
    50%     { transform: translate(-40px, 60px) scale(1.15); }
  }

  .grid {
    position: absolute;
    inset: 0;
    background-image:
      linear-gradient(rgba(255,255,255,0.025) 1px, transparent 1px),
      linear-gradient(90deg, rgba(255,255,255,0.025) 1px, transparent 1px);
    background-size: 44px 44px;
    mask-image: radial-gradient(ellipse 70% 60% at 50% 40%, black 30%, transparent 80%);
    -webkit-mask-image: radial-gradient(ellipse 70% 60% at 50% 40%, black 30%, transparent 80%);
  }

  .wrap {
    position: relative;
    z-index: 1;
    max-width: 760px;
    margin: 0 auto;
    padding: 22px 18px 40px;
    min-height: 100vh;
    min-height: 100dvh;
    display: flex;
    flex-direction: column;
    gap: 18px;
  }

  header {
    display: flex;
    align-items: center;
    justify-content: space-between;
    gap: 12px;
    animation: fadeUp 0.6s cubic-bezier(0.22, 1, 0.36, 1) both;
  }

  .logo {
    display: flex;
    align-items: center;
    gap: 12px;
    font-size: 1.35rem;
    font-weight: 800;
    letter-spacing: -0.02em;
    background: linear-gradient(90deg, #ffffff 0%, #cfd4ff 60%, var(--accent) 100%);
    -webkit-background-clip: text;
    background-clip: text;
    color: transparent;
  }

  .logo-mark {
    position: relative;
    width: 32px;
    height: 32px;
    border-radius: 10px;
    background: conic-gradient(from 210deg, var(--accent), var(--accent-2), var(--accent-3), var(--accent));
    display: grid;
    place-items: center;
    box-shadow:
      0 0 0 1px rgba(255,255,255,0.15) inset,
      0 0 22px rgba(0, 224, 255, 0.4);
    animation: spinSlow 8s linear infinite;
  }
  .logo-mark::after {
    content: '';
    width: 12px; height: 12px;
    border-radius: 4px;
    background: var(--bg-0);
    box-shadow: 0 0 0 1px rgba(255,255,255,0.12) inset;
  }
  @keyframes spinSlow { to { transform: rotate(360deg); } }

  .status-pill {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    font-size: 0.72rem;
    font-weight: 600;
    letter-spacing: 0.06em;
    text-transform: uppercase;
    padding: 8px 14px;
    border-radius: 999px;
    background: var(--glass);
    border: 1px solid var(--border);
    backdrop-filter: blur(14px) saturate(160%);
    -webkit-backdrop-filter: blur(14px) saturate(160%);
    color: var(--muted);
    transition: color 0.3s ease, border-color 0.3s ease;
  }
  .status-pill .dot {
    position: relative;
    width: 8px; height: 8px;
    border-radius: 50%;
    background: var(--good);
    box-shadow: 0 0 10px var(--good);
  }
  .status-pill .dot::after {
    content: '';
    position: absolute;
    inset: -3px;
    border-radius: 50%;
    border: 1px solid var(--good);
    opacity: 0.6;
    animation: ringPulse 1.8s ease-out infinite;
  }
  @keyframes ringPulse {
    0%   { transform: scale(0.7); opacity: 0.8; }
    100% { transform: scale(1.8); opacity: 0;   }
  }
  .status-pill.offline { color: var(--bad); border-color: rgba(255, 69, 96, 0.3); }
  .status-pill.offline .dot { background: var(--bad); box-shadow: 0 0 10px var(--bad); }
  .status-pill.offline .dot::after { border-color: var(--bad); }

  .stats {
    display: grid;
    grid-template-columns: 1fr 1fr 1fr;
    gap: 12px;
    animation: fadeUp 0.6s cubic-bezier(0.22, 1, 0.36, 1) 0.05s both;
  }
  .stat {
    position: relative;
    padding: 14px 14px 13px;
    border-radius: var(--radius-md);
    background: var(--glass);
    border: 1px solid var(--border);
    backdrop-filter: blur(16px) saturate(160%);
    -webkit-backdrop-filter: blur(16px) saturate(160%);
    overflow: hidden;
    transition: border-color 0.3s ease, transform 0.3s ease;
  }
  .stat::before {
    content: '';
    position: absolute;
    inset: 0 0 auto 0;
    height: 2px;
    background: linear-gradient(90deg, transparent, var(--accent), transparent);
    opacity: 0.75;
  }
  .stat--score::before { background: linear-gradient(90deg, transparent, var(--gold), transparent); }
  .stat--total::before { background: linear-gradient(90deg, transparent, var(--accent-2), transparent); }

  .stat-label {
    font-size: 0.66rem;
    font-weight: 700;
    letter-spacing: 0.16em;
    text-transform: uppercase;
    color: var(--faint);
    margin-bottom: 4px;
  }
  .stat-value {
    font-size: 1.45rem;
    font-weight: 800;
    letter-spacing: -0.02em;
    color: var(--text);
    font-variant-numeric: tabular-nums;
    line-height: 1.1;
  }
  .stat--q .stat-value      { color: var(--accent); text-shadow: 0 0 20px rgba(0,224,255,0.35); }
  .stat--score .stat-value  { color: var(--gold);   text-shadow: 0 0 20px rgba(255,204,77,0.35); }
  .stat--total .stat-value  { color: #d7c4ff;       text-shadow: 0 0 20px rgba(177,108,255,0.3); }

  .progress-track {
    position: relative;
    height: 10px;
    width: 100%;
    border-radius: 999px;
    background: rgba(255,255,255,0.06);
    border: 1px solid var(--border);
    overflow: hidden;
    animation: fadeUp 0.6s cubic-bezier(0.22, 1, 0.36, 1) 0.1s both;
  }
  .progress-fill {
    position: relative;
    height: 100%;
    width: 0%;
    border-radius: 999px;
    background: linear-gradient(90deg, var(--accent), var(--accent-2) 60%, var(--accent-3));
    box-shadow:
      0 0 14px rgba(0, 224, 255, 0.6),
      0 0 30px rgba(177, 108, 255, 0.4);
    transition: width 0.55s cubic-bezier(0.22, 1, 0.36, 1);
    overflow: hidden;
  }
  .progress-fill::before {
    content: '';
    position: absolute;
    inset: 0;
    background: linear-gradient(90deg,
      transparent 0%,
      rgba(255,255,255,0.45) 50%,
      transparent 100%);
    animation: sheen 2.4s linear infinite;
  }
  @keyframes sheen {
    0%   { transform: translateX(-100%); }
    100% { transform: translateX(100%); }
  }
  .progress-fill::after {
    content: '';
    position: absolute;
    right: -1px; top: 50%;
    width: 14px; height: 14px;
    transform: translateY(-50%);
    border-radius: 50%;
    background: #ffffff;
    box-shadow: 0 0 12px #ffffff, 0 0 26px var(--accent);
    opacity: 0.95;
  }

  .card {
    position: relative;
    flex: 1;
    padding: 26px 24px;
    border-radius: var(--radius-lg);
    background:
      linear-gradient(180deg, rgba(255,255,255,0.055) 0%, rgba(255,255,255,0.02) 100%);
    border: 1px solid var(--border-2);
    backdrop-filter: blur(22px) saturate(160%);
    -webkit-backdrop-filter: blur(22px) saturate(160%);
    box-shadow:
      var(--shadow-lg),
      0 0 0 1px rgba(255,255,255,0.03) inset,
      0 1px 0 rgba(255,255,255,0.14) inset;
    display: flex;
    flex-direction: column;
    gap: 20px;
    overflow: hidden;
    animation: fadeUp 0.6s cubic-bezier(0.22, 1, 0.36, 1) 0.14s both;
  }
  .card::before {
    content: '';
    position: absolute;
    inset: -1px -1px auto -1px;
    height: 1px;
    background: linear-gradient(90deg,
      transparent,
      var(--accent) 25%,
      var(--accent-2) 50%,
      var(--accent-3) 75%,
      transparent);
    opacity: 0.85;
    filter: blur(0.3px);
    animation: hueShift 8s linear infinite;
  }
  @keyframes hueShift {
    0%,100% { filter: hue-rotate(0deg); }
    50%     { filter: hue-rotate(40deg); }
  }
  .card::after {
    content: '';
    position: absolute;
    top: -80px; right: -80px;
    width: 220px; height: 220px;
    background: radial-gradient(circle, rgba(0,224,255,0.18), transparent 70%);
    pointer-events: none;
  }

  .idle-screen,
  .end-screen {
    display: none;
    flex-direction: column;
    align-items: center;
    justify-content: center;
    gap: 22px;
    text-align: center;
    padding: 26px 8px;
    flex: 1;
  }
  .idle-screen.active,
  .end-screen.active { display: flex; }

  .hero-badge {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    font-size: 0.68rem;
    font-weight: 700;
    letter-spacing: 0.2em;
    text-transform: uppercase;
    padding: 7px 14px;
    border-radius: 999px;
    color: var(--accent);
    background: rgba(0, 224, 255, 0.08);
    border: 1px solid rgba(0, 224, 255, 0.28);
    box-shadow: inset 0 1px 0 rgba(255,255,255,0.06);
  }
  .hero-badge::before {
    content: '';
    width: 6px; height: 6px;
    border-radius: 50%;
    background: var(--accent);
    box-shadow: 0 0 10px var(--accent);
    animation: pulse 1.6s ease-in-out infinite;
  }
  @keyframes pulse {
    0%,100% { transform: scale(1); opacity: 1; }
    50%     { transform: scale(1.4); opacity: 0.6; }
  }

  .big {
    font-size: clamp(2.4rem, 8vw, 3.6rem);
    font-weight: 900;
    letter-spacing: -0.04em;
    line-height: 1.05;
    background: linear-gradient(120deg, #ffffff 0%, #cfd4ff 40%, var(--accent) 75%, var(--accent-2) 100%);
    background-size: 200% 200%;
    -webkit-background-clip: text;
    background-clip: text;
    color: transparent;
    animation: gradientFlow 6s ease infinite;
    text-align: center;
  }
  @keyframes gradientFlow {
    0%,100% { background-position: 0% 50%; }
    50%     { background-position: 100% 50%; }
  }

  .hint {
    color: var(--muted);
    font-size: 0.98rem;
    line-height: 2;
    max-width: 460px;
  }
  .hint strong { color: var(--text); font-weight: 700; }
  .hint .row {
    display: flex;
    align-items: center;
    justify-content: center;
    flex-wrap: wrap;
    gap: 6px;
    margin: 2px 0;
  }

  .keycap {
    display: inline-flex;
    align-items: center;
    justify-content: center;
    min-width: 26px;
    height: 26px;
    padding: 0 8px;
    border-radius: 8px;
    font-family: var(--font-mono);
    font-size: 0.85rem;
    font-weight: 700;
    color: var(--accent);
    background:
      linear-gradient(180deg, rgba(0,224,255,0.14) 0%, rgba(0,224,255,0.06) 100%);
    border: 1px solid rgba(0, 224, 255, 0.35);
    box-shadow:
      0 1px 0 rgba(255,255,255,0.12) inset,
      0 2px 6px rgba(0, 0, 0, 0.35),
      0 0 12px rgba(0, 224, 255, 0.15);
    vertical-align: middle;
    margin: 0 1px;
    transition: transform 0.15s ease, box-shadow 0.2s ease;
  }
  .keycap:hover {
    transform: translateY(-1px);
    box-shadow:
      0 1px 0 rgba(255,255,255,0.16) inset,
      0 4px 10px rgba(0, 0, 0, 0.4),
      0 0 18px rgba(0, 224, 255, 0.35);
  }
  .keycap.wide { padding: 0 14px; }
  .keycap.accent {
    color: var(--accent-2);
    border-color: rgba(177, 108, 255, 0.4);
    background: linear-gradient(180deg, rgba(177,108,255,0.16) 0%, rgba(177,108,255,0.06) 100%);
    box-shadow:
      0 1px 0 rgba(255,255,255,0.12) inset,
      0 2px 6px rgba(0,0,0,0.35),
      0 0 12px rgba(177,108,255,0.2);
  }

  #questionScreen {
    display: none;
    flex-direction: column;
    gap: 20px;
    animation: fadeUp 0.4s cubic-bezier(0.22, 1, 0.36, 1);
  }

  .q-type {
    display: inline-flex;
    align-items: center;
    gap: 8px;
    font-size: 0.66rem;
    font-weight: 800;
    letter-spacing: 0.18em;
    text-transform: uppercase;
    padding: 6px 12px;
    border-radius: 999px;
    color: #e2d5ff;
    background: linear-gradient(180deg, rgba(177,108,255,0.22) 0%, rgba(177,108,255,0.08) 100%);
    border: 1px solid rgba(177, 108, 255, 0.4);
    box-shadow: inset 0 1px 0 rgba(255,255,255,0.08);
    width: fit-content;
  }
  .q-type::before {
    content: '';
    width: 6px; height: 6px;
    border-radius: 50%;
    background: var(--accent-2);
    box-shadow: 0 0 10px var(--accent-2);
  }

  .question {
    font-size: clamp(1.15rem, 3.4vw, 1.45rem);
    font-weight: 700;
    line-height: 1.45;
    letter-spacing: -0.01em;
    color: var(--text);
    padding: 4px 2px;
    min-height: 68px;
  }

  .options {
    display: grid;
    grid-template-columns: 1fr 1fr;
    gap: 12px;
  }
  .option {
    position: relative;
    display: flex;
    align-items: center;
    gap: 12px;
    padding: 16px 16px;
    border-radius: var(--radius-md);
    background: linear-gradient(180deg, rgba(255,255,255,0.055), rgba(255,255,255,0.02));
    border: 1px solid var(--border-2);
    font-size: 0.98rem;
    font-weight: 500;
    color: var(--text);
    overflow: hidden;
    transition:
      transform 0.25s cubic-bezier(0.22, 1, 0.36, 1),
      border-color 0.25s ease,
      background 0.25s ease,
      box-shadow 0.25s ease;
  }
  .option::before {
    content: '';
    position: absolute;
    inset: 0;
    background: radial-gradient(120% 100% at 0% 0%, rgba(0,224,255,0.16), transparent 55%);
    opacity: 0;
    transition: opacity 0.3s ease;
    pointer-events: none;
  }
  .option:hover {
    transform: translateY(-2px);
    border-color: rgba(255,255,255,0.24);
    box-shadow: 0 10px 24px -10px rgba(0,0,0,0.6);
  }
  .option:hover::before { opacity: 1; }

  .option .key {
    flex-shrink: 0;
    display: inline-flex;
    align-items: center;
    justify-content: center;
    width: 36px; height: 36px;
    border-radius: 11px;
    font-family: var(--font-mono);
    font-size: 0.95rem;
    font-weight: 800;
    color: var(--accent);
    background: linear-gradient(180deg, rgba(0,224,255,0.16) 0%, rgba(0,224,255,0.05) 100%);
    border: 1px solid rgba(0, 224, 255, 0.4);
    box-shadow:
      0 1px 0 rgba(255,255,255,0.12) inset,
      0 0 14px rgba(0, 224, 255, 0.2);
    transition: all 0.25s ease;
  }
  .option .opt-text { flex: 1; line-height: 1.3; }

  .option.selected {
    border-color: var(--accent);
    background: linear-gradient(180deg, rgba(0,224,255,0.18), rgba(0,224,255,0.05));
    box-shadow:
      0 0 0 1px rgba(0, 224, 255, 0.4) inset,
      0 10px 30px -10px rgba(0, 224, 255, 0.6),
      0 0 24px rgba(0, 224, 255, 0.25);
    transform: translateY(-2px);
  }
  .option.selected .key {
    background: linear-gradient(180deg, var(--accent) 0%, #00b4d8 100%);
    color: #05202a;
    border-color: transparent;
    box-shadow: 0 0 22px rgba(0, 224, 255, 0.7);
  }
  .option.correct {
    border-color: var(--good);
    background: linear-gradient(180deg, rgba(25,247,165,0.2), rgba(25,247,165,0.06));
    box-shadow:
      0 0 0 1px rgba(25, 247, 165, 0.45) inset,
      0 10px 30px -10px rgba(25, 247, 165, 0.6),
      0 0 24px rgba(25, 247, 165, 0.28);
    transform: translateY(-2px);
  }
  .option.correct .key {
    background: linear-gradient(180deg, var(--good) 0%, #0ecf85 100%);
    color: #052a1c;
    border-color: transparent;
    box-shadow: 0 0 22px rgba(25, 247, 165, 0.7);
  }
  .option.wrong {
    border-color: var(--bad);
    background: linear-gradient(180deg, rgba(255,69,96,0.2), rgba(255,69,96,0.06));
    box-shadow:
      0 0 0 1px rgba(255, 69, 96, 0.45) inset,
      0 10px 30px -10px rgba(255, 69, 96, 0.55);
    animation: shake 0.4s cubic-bezier(0.36, 0.07, 0.19, 0.97);
  }
  .option.wrong .key {
    background: linear-gradient(180deg, var(--bad) 0%, #cc1b3a 100%);
    color: #ffffff;
    border-color: transparent;
    box-shadow: 0 0 22px rgba(255, 69, 96, 0.7);
  }
  @keyframes shake {
    10%,90% { transform: translateX(-1px); }
    20%,80% { transform: translateX(2px); }
    30%,50%,70% { transform: translateX(-4px); }
    40%,60% { transform: translateX(4px); }
  }

  .answer-display {
    display: flex;
    flex-direction: column;
    gap: 10px;
    align-items: center;
    padding: 24px 20px;
    border-radius: var(--radius-md);
    background:
      radial-gradient(120% 100% at 50% 0%, rgba(0,224,255,0.08), transparent 60%),
      rgba(0, 0, 0, 0.35);
    border: 1px solid var(--border-2);
    box-shadow:
      inset 0 2px 20px rgba(0, 0, 0, 0.5),
      inset 0 0 0 1px rgba(255,255,255,0.03);
  }
  .answer-display .label {
    font-size: 0.68rem;
    font-weight: 700;
    letter-spacing: 0.22em;
    text-transform: uppercase;
    color: var(--faint);
  }
  .answer-display .buffer {
    font-family: var(--font-mono);
    font-size: clamp(2rem, 8vw, 2.8rem);
    font-weight: 700;
    letter-spacing: 0.15em;
    color: var(--accent);
    text-shadow:
      0 0 14px rgba(0, 224, 255, 0.8),
      0 0 30px rgba(0, 224, 255, 0.45);
    min-height: 52px;
    display: flex;
    align-items: center;
    justify-content: center;
    font-variant-numeric: tabular-nums;
  }
  .answer-display .buffer .caret {
    display: inline-block;
    width: 3px;
    height: 34px;
    margin-left: 6px;
    border-radius: 2px;
    background: var(--accent);
    box-shadow: 0 0 12px var(--accent);
    animation: blink 1.05s steps(1) infinite;
    vertical-align: middle;
  }
  @keyframes blink {
    0%, 49%  { opacity: 1; }
    50%,100% { opacity: 0; }
  }

  .feedback {
    display: none;
    text-align: center;
    font-size: 1rem;
    font-weight: 700;
    padding: 14px 16px;
    border-radius: var(--radius-md);
    line-height: 1.5;
    letter-spacing: 0.005em;
    animation: popIn 0.35s cubic-bezier(0.22, 1, 0.36, 1);
  }
  @keyframes popIn {
    0%   { transform: scale(0.96); opacity: 0; }
    100% { transform: scale(1);    opacity: 1; }
  }
  .feedback.good {
    display: block;
    color: var(--good);
    background:
      linear-gradient(180deg, rgba(25,247,165,0.14), rgba(25,247,165,0.04));
    border: 1px solid rgba(25, 247, 165, 0.4);
    box-shadow:
      inset 0 1px 0 rgba(255,255,255,0.08),
      0 0 30px rgba(25, 247, 165, 0.15);
  }
  .feedback.bad {
    display: block;
    color: var(--bad);
    background:
      linear-gradient(180deg, rgba(255,69,96,0.14), rgba(255,69,96,0.04));
    border: 1px solid rgba(255, 69, 96, 0.4);
    box-shadow:
      inset 0 1px 0 rgba(255,255,255,0.06),
      0 0 30px rgba(255, 69, 96, 0.15);
  }
  .feedback .next-hint {
    display: block;
    font-size: 0.78rem;
    font-weight: 500;
    letter-spacing: 0.02em;
    color: var(--muted);
    margin-top: 8px;
  }

  footer {
    text-align: center;
    font-size: 0.72rem;
    letter-spacing: 0.08em;
    color: var(--faint);
    padding-top: 6px;
    animation: fadeUp 0.6s cubic-bezier(0.22, 1, 0.36, 1) 0.2s both;
  }
  footer .dot-sep { display: inline-block; margin: 0 8px; opacity: 0.5; }

  @keyframes fadeUp {
    from { opacity: 0; transform: translateY(10px); }
    to   { opacity: 1; transform: translateY(0); }
  }

  @media (max-width: 520px) {
    .wrap { padding: 16px 14px 30px; gap: 14px; }
    .card { padding: 20px 16px; border-radius: 20px; }
    .options { grid-template-columns: 1fr; }
    .stat-value { font-size: 1.25rem; }
    .stat { padding: 12px 12px 11px; }
    .logo { font-size: 1.2rem; }
    .hint { font-size: 0.92rem; line-height: 1.9; }
  }

  @media (prefers-reduced-motion: reduce) {
    *, *::before, *::after {
      animation-duration: 0.001ms !important;
      animation-iteration-count: 1 !important;
      transition-duration: 0.001ms !important;
    }
  }
</style>
</head>
<body>

<div class="bg" aria-hidden="true">
  <div class="orb orb-1"></div>
  <div class="orb orb-2"></div>
  <div class="orb orb-3"></div>
  <div class="grid"></div>
</div>

<div class="wrap">

  <header>
    <div class="logo">
      <span class="logo-mark"></span>
      MindMatrix
    </div>
    <div class="status-pill" id="statusPill">
      <span class="dot"></span>
      <span id="statusText">Connected</span>
    </div>
  </header>

  <div class="stats">
    <div class="stat stat--q">
      <div class="stat-label">Question</div>
      <div class="stat-value" id="qNum">–</div>
    </div>
    <div class="stat stat--score">
      <div class="stat-label">Score</div>
      <div class="stat-value" id="score">0</div>
    </div>
    <div class="stat stat--total">
      <div class="stat-label">Total</div>
      <div class="stat-value" id="total">50</div>
    </div>
  </div>

  <div class="progress-track">
    <div class="progress-fill" id="progressFill"></div>
  </div>

  <main class="card">

    <div class="idle-screen active" id="idleScreen">
      <div class="hero-badge">Ready to play</div>
      <div class="big">MindMatrix</div>
      <div class="hint">
        <div class="row">
          Press <span class="keycap">#</span> to start the quiz.
        </div>
        <div class="row">
          <strong>MCQ</strong> &nbsp;·&nbsp;
          <span class="keycap">A</span>
          <span class="keycap">B</span>
          <span class="keycap">C</span>
          <span class="keycap">D</span>
        </div>
        <div class="row">
          <strong>Numeric</strong> &nbsp;·&nbsp; type <span class="keycap">0</span>–<span class="keycap">9</span> then <span class="keycap">#</span>
        </div>
        <div class="row">
          <span class="keycap">#</span> next &nbsp;·&nbsp;
          <span class="keycap accent">*</span> back / clear
        </div>
      </div>
    </div>

    <div id="questionScreen">
      <div class="q-type" id="qType">MCQ</div>
      <div class="question" id="questionText">Loading…</div>

      <div class="options" id="optionsBox">
        <div class="option" data-idx="0"><span class="key">A</span><span class="opt-text">–</span></div>
        <div class="option" data-idx="1"><span class="key">B</span><span class="opt-text">–</span></div>
        <div class="option" data-idx="2"><span class="key">C</span><span class="opt-text">–</span></div>
        <div class="option" data-idx="3"><span class="key">D</span><span class="opt-text">–</span></div>
      </div>

      <div class="answer-display" id="answerBox" style="display:none;">
        <div class="label">Your Answer</div>
        <div class="buffer" id="answerBuffer">
          <span class="caret"></span>
        </div>
      </div>

      <div class="feedback" id="feedback"></div>
    </div>

    <div class="end-screen" id="endScreen">
      <div class="hero-badge">Session complete</div>
      <div class="big">Well played!</div>
      <div class="hint">
        Your final score is
        <span class="keycap" id="finalScore">0</span>
        out of
        <span class="keycap accent" id="finalTotal">50</span>
        <div class="row" style="margin-top: 18px;">
          Press <span class="keycap">#</span> to play again
        </div>
      </div>
    </div>

  </main>

  <footer>
    MindMatrix <span class="dot-sep">·</span> ATL Lab <span class="dot-sep">·</span> ESP8266 D1 mini
  </footer>
</div>

<script>
const API_URL = "/api/state";
const POLL_MS = 500;

const el = {
  statusPill:     document.getElementById("statusPill"),
  statusText:     document.getElementById("statusText"),
  qNum:           document.getElementById("qNum"),
  score:          document.getElementById("score"),
  total:          document.getElementById("total"),
  progressFill:   document.getElementById("progressFill"),
  idleScreen:     document.getElementById("idleScreen"),
  questionScreen: document.getElementById("questionScreen"),
  endScreen:      document.getElementById("endScreen"),
  qType:          document.getElementById("qType"),
  questionText:   document.getElementById("questionText"),
  optionsBox:     document.getElementById("optionsBox"),
  answerBox:      document.getElementById("answerBox"),
  answerBuffer:   document.getElementById("answerBuffer"),
  feedback:       document.getElementById("feedback"),
  finalScore:     document.getElementById("finalScore"),
  finalTotal:     document.getElementById("finalTotal"),
};

let lastStateStr = "";

async function poll() {
  try {
    const res  = await fetch(API_URL, { cache: "no-store" });
    const data = await res.json();
    el.statusPill.classList.remove("offline");
    el.statusText.textContent = "Connected";
    render(data);
  } catch (err) {
    el.statusPill.classList.add("offline");
    el.statusText.textContent = "Disconnected";
  }
}

function render(s) {
  const key = JSON.stringify(s);
  if (key === lastStateStr) return;
  lastStateStr = key;

  const total  = s.total || 50;
  const qIndex = s.index || 0;
  const score  = s.score || 0;

  el.total.textContent = total;
  el.score.textContent = score;
  el.qNum.textContent  = Math.min(qIndex + 1, total) + "/" + total;

  const pct = total > 0 ? (qIndex / total) * 100 : 0;
  el.progressFill.style.width = pct + "%";

  el.idleScreen.classList.remove("active");
  el.questionScreen.style.display = "none";
  el.endScreen.classList.remove("active");

  if (s.state === "idle") {
    el.idleScreen.classList.add("active");
    el.qNum.textContent = "–";
    el.progressFill.style.width = "0%";
    return;
  }

  if (s.state === "finished" || s.state === "end") {
    el.endScreen.classList.add("active");
    el.finalScore.textContent = score;
    el.finalTotal.textContent = total;
    el.progressFill.style.width = "100%";
    return;
  }

  el.questionScreen.style.display = "flex";
  el.qType.textContent = (s.type === "num") ? "Numeric Answer" : "Multiple Choice";
  el.questionText.textContent = s.question || "…";

  if (s.type === "mcq") {
    el.optionsBox.style.display = "grid";
    el.answerBox.style.display  = "none";

    const opts = s.options || [];
    const optionEls = el.optionsBox.querySelectorAll(".option");

    optionEls.forEach((node, i) => {
      const txt = node.querySelector(".opt-text");
      txt.textContent = opts[i] !== undefined ? opts[i] : "–";

      node.classList.remove("selected", "correct", "wrong");

      if (s.state === "feedback") {
        if (i === s.correctIndex) node.classList.add("correct");
        if (i === s.selectedIndex && i !== s.correctIndex) node.classList.add("wrong");
      } else if (s.selectedIndex === i) {
        node.classList.add("selected");
      }
    });
  } else {
    el.optionsBox.style.display = "none";
    el.answerBox.style.display  = "flex";

    const buf = (s.buffer || "").toString();
    el.answerBuffer.innerHTML = buf.length
      ? escapeHtml(buf) + '<span class="caret"></span>'
      : '<span class="caret"></span>';
  }

  if (s.state === "feedback") {
    const nextHint = '<span class="next-hint">Press <span class="keycap">#</span> for next &nbsp;·&nbsp; <span class="keycap accent">*</span> to go back</span>';

    if (s.correct) {
      el.feedback.className = "feedback good";
      el.feedback.innerHTML = "&#10004; Correct!" + nextHint;
    } else {
      el.feedback.className = "feedback bad";
      el.feedback.innerHTML =
        "&#10008; Wrong — Answer: " + escapeHtml(s.correctAnswer || "") + nextHint;
    }
  } else {
    el.feedback.className = "feedback";
    el.feedback.innerHTML = "";
  }
}

function escapeHtml(str) {
  return String(str)
    .replace(/&/g, "&amp;")
    .replace(/</g, "&lt;")
    .replace(/>/g, "&gt;");
}

poll();
setInterval(poll, POLL_MS);
</script>
</body>
</html>
)RAWHTML";

#endif