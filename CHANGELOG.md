# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- This CHANGELOG to keep track on game changes.
- A **GAME OVER state**. If the user clicks on a mine, mouse clicks will not have effect anymore until the game is restarted.

### Changed

- The sequence of events for having a new game. Now the mines are distributed AFTER the first click, which guarantees the first opened cells is not a mine.

## [0.1.0] - 2024-05-05

### Added

- 3 game Levels: Beginner, Intermediate and Advanced
- Possibility to **Flag/Unflag** the cells the player think have a mine.
- **Chording**, which automatically reveal adjacent cells.
- README with instructions for playing the game and building it from source.

[unreleased]: https://github.com/henrimitte/AMC/compare/0.1.0...HEAD
[0.1.0]: https://github.com/henrimitte/AMC/releases/tag/0.1.0