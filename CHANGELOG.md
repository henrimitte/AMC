# Changelog

All notable changes to this project will be documented in this file.

The format is based on [Keep a Changelog](https://keepachangelog.com/en/1.1.0/),
and this project adheres to [Semantic Versioning](https://semver.org/spec/v2.0.0.html).

## [Unreleased]

### Added

- Sprites, giving a visual improvement to the game!

## [0.3.1] - 2024-05-16

### Fixed

- Flagged mines are not revealed anymore on a Game Over.

## [0.3.0] - 2024-05-16

###  Added

- All mines are revealed when a Game Over state happens.

### Fixed

- If a cell is flagged, it will not open when clicked.

## [0.2.1] - 2024-05-09

### Fixed

- When chording, if a mine was revealed, the game would not stop. Now it correctly set the state to Game Over in those cases.

## [0.2.0] - 2024-05-09

### Added

- This CHANGELOG to keep track of game changes.
- A Game Over state. If the user clicks on a mine, mouse clicks will not have effect anymore until the game is restarted.

### Changed

- README file to describe the new features.
- The sequence of events for having a new game. Now the mines are distributed AFTER the first click, which guarantees the first opened cells is not a mine.

## [0.1.0] - 2024-05-05

### Added

- 3 game Levels: Beginner, Intermediate and Advanced
- Possibility to **Flag/Unflag** the cells the player think have a mine.
- **Chording**, which automatically reveal adjacent cells.
- README with instructions for playing the game and building it from source.

[Unreleased]: https://github.com/henrimitte/AMC/compare/v0.3.1...HEAD
[0.3.1]: https://github.com/henrimitte/AMC/compare/v0.3.0...v0.3.1
[0.3.0]: https://github.com/henrimitte/AMC/compare/v0.2.1...v0.3.0
[0.2.1]: https://github.com/henrimitte/AMC/compare/v0.2.0...v0.2.1
[0.2.0]: https://github.com/henrimitte/AMC/compare/v0.1.0...v0.2.0
[0.1.0]: https://github.com/henrimitte/AMC/releases/tag/v0.1.0
