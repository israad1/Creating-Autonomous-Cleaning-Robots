#pragma once
#include <string>
#include <iostream>
#include <sstream>
#include <iomanip>
#include <cmath>
#include <algorithm>

// ──────────────────────────────────────────────────────────────
//  UI  –  Terminal UI utilities for CleanBot Fleet Manager
//  Header-only; no build changes needed.
// ──────────────────────────────────────────────────────────────
namespace UI {

// ── ANSI styles ────────────────────────────────────────────────
inline const std::string RST    = "\033[0m";
inline const std::string BOLD   = "\033[1m";
inline const std::string DIM    = "\033[2m";

inline const std::string RED    = "\033[31m";
inline const std::string GREEN  = "\033[32m";
inline const std::string YELLOW = "\033[33m";
inline const std::string BLUE   = "\033[34m";
inline const std::string MAGENTA= "\033[35m";
inline const std::string CYAN   = "\033[36m";
inline const std::string WHITE  = "\033[37m";

inline const std::string BRED   = "\033[91m";
inline const std::string BGRN   = "\033[92m";
inline const std::string BYEL   = "\033[93m";
inline const std::string BBLU   = "\033[94m";
inline const std::string BMAG   = "\033[95m";
inline const std::string BCYN   = "\033[96m";
inline const std::string BWHT   = "\033[97m";

// ── Helpers ────────────────────────────────────────────────────
inline void error(const std::string& msg) {
    std::cout << BRED << BOLD << "  [!] " << RST << RED   << msg << RST << "\n";
}
inline void success(const std::string& msg) {
    std::cout << BGRN << BOLD << "  [+] " << RST << GREEN  << msg << RST << "\n";
}
inline void info(const std::string& msg) {
    std::cout << BCYN << BOLD << "  [i] " << RST << CYAN   << msg << RST << "\n";
}
inline void warn(const std::string& msg) {
    std::cout << BYEL << BOLD << "  [~] " << RST << YELLOW << msg << RST << "\n";
}

// ── Progress bar ───────────────────────────────────────────────
// Returns a coloured bar string; width is number of block chars.
inline std::string bar(float pct, int width = 14) {
    if (std::isnan(pct)) {
        return DIM + std::string(static_cast<size_t>(width), '-') + RST;
    }
    float clamped = std::clamp(pct, 0.0f, 100.0f);
    int filled = static_cast<int>(clamped / 100.0f * static_cast<float>(width));
    std::string col;
    if      (clamped >= 100.0f) col = BGRN;
    else if (clamped >= 60.0f)  col = GREEN;
    else if (clamped >= 30.0f)  col = YELLOW;
    else                         col = BRED;

    std::string result = col;
    for (int i = 0; i < width; ++i) {
        result += (i < filled) ? "\u2588" : "\u2591";   // █  ░
    }
    result += RST;
    return result;
}

// Battery bar (same as bar but label-aware)
inline std::string battBar(float pct, int width = 10) {
    return bar(pct, width);
}

// Percentage string, right-aligned in 6 chars, coloured
inline std::string pctStr(float pct) {
    if (std::isnan(pct)) return DIM + "  N/A " + RST;
    std::ostringstream ss;
    std::string col;
    if      (pct >= 100.0f) col = BGRN;
    else if (pct >= 60.0f)  col = GREEN;
    else if (pct >= 30.0f)  col = YELLOW;
    else                     col = BRED;
    ss << col << std::setw(5) << std::fixed << std::setprecision(1) << pct << "%" << RST;
    return ss.str();
}

// ── Status badge ───────────────────────────────────────────────
inline std::string statusBadge(bool busy, bool failed, bool atBase) {
    if (failed)  return BRED   + BOLD + " FAILED  " + RST;
    if (busy)    return BYEL   + BOLD + " WORKING " + RST;
    if (atBase)  return BCYN   + BOLD + " CHARGING" + RST;
    return              BGRN   + BOLD + " READY   " + RST;
}

// ── Room cleanliness label ─────────────────────────────────────
inline std::string roomStatus(float swept, float mopped, float scrubbed, float vacuumed) {
    // Any applicable metric below 100 => dirty
    bool dirty = false;
    if (!std::isnan(swept)    && swept    < 100.0f) dirty = true;
    if (!std::isnan(mopped)   && mopped   < 100.0f) dirty = true;
    if (!std::isnan(scrubbed) && scrubbed < 100.0f) dirty = true;
    if (!std::isnan(vacuumed) && vacuumed < 100.0f) dirty = true;
    return dirty ? (RED + BOLD + " DIRTY " + RST) : (BGRN + BOLD + " CLEAN " + RST);
}

// ── Dividers ───────────────────────────────────────────────────
inline void hline(int w = 72, const std::string& col = "\033[36m") {
    std::cout << col;
    for (int i = 0; i < w; ++i) std::cout << "\u2500";   // ─
    std::cout << RST << "\n";
}

inline void thickLine(int w = 72, const std::string& col = "\033[36m") {
    std::cout << col;
    for (int i = 0; i < w; ++i) std::cout << "\u2550";   // ═
    std::cout << RST << "\n";
}

// ── Section header ─────────────────────────────────────────────
inline void section(const std::string& title, const std::string& col = "\033[96m") {
    std::cout << "\n" << col << BOLD << "  \u25b6 " << title << RST << "\n";
    hline(70, col);
}

// ── Column helpers ─────────────────────────────────────────────
inline std::string col(const std::string& s, int w) {
    // left-pad with spaces up to width w (ignoring ANSI codes in length)
    // Simple version: just pad raw string
    if ((int)s.size() < w) return s + std::string(static_cast<size_t>(w - (int)s.size()), ' ');
    return s;
}

// padded plain column (no ANSI)
inline std::string pcol(const std::string& s, int w) {
    std::string r = s;
    if ((int)r.size() > w) r = r.substr(0, static_cast<size_t>(w));
    while ((int)r.size() < w) r += ' ';
    return r;
}

// ── Welcome banner ─────────────────────────────────────────────
inline void banner() {
    std::cout << BCYN << BOLD << R"(
  ╔══════════════════════════════════════════════════════════════════╗
  ║                                                                  ║
  ║    ██████╗██╗     ███████╗ █████╗ ███╗  ██╗██████╗  ██████╗ ████║
  ║   ██╔════╝██║     ██╔════╝██╔══██╗████╗ ██║██╔══██╗██╔═══██╗╚══██║
  ║   ██║     ██║     █████╗  ███████║██╔██╗██║██████╔╝██║   ██║  ██╔╝
  ║   ██║     ██║     ██╔══╝  ██╔══██║██║╚████║██╔══██╗██║   ██║ ██╔╝ ║
  ║   ╚██████╗███████╗███████╗██║  ██║██║ ╚███║██████╔╝╚██████╔╝██████║
  ║    ╚═════╝╚══════╝╚══════╝╚═╝  ╚═╝╚═╝  ╚══╝╚═════╝  ╚═════╝ ╚════╝
  ║                                                                  ║
  ║         Autonomous Robot Fleet Manager  ─  v2.0                  ║
  ║         Type  help  for available commands                       ║
  ╚══════════════════════════════════════════════════════════════════╝
)" << RST;
}

// ── Command prompt ─────────────────────────────────────────────
inline void prompt(int time) {
    std::cout << "\n" << DIM << "  [T=" << std::setw(4) << time << "] "
              << RST << BCYN << BOLD << "fleet> " << RST;
}

} // namespace UI
