(() => {
  const boardElement = document.getElementById('game-board');
  const statusElement = document.getElementById('game-status');
  const resetButton = document.getElementById('reset-game');
  const lines = [
    [0, 1, 2], [3, 4, 5], [6, 7, 8],
    [0, 3, 6], [1, 4, 7], [2, 5, 8],
    [0, 4, 8], [2, 4, 6]
  ];
  let cells = Array(9).fill('');
  let turn = 'X';
  let finished = false;

  function render() {
    boardElement.replaceChildren();
    cells.forEach((mark, index) => {
      const button = document.createElement('button');
      button.type = 'button';
      button.textContent = mark === 'X' ? '×' : mark === 'O' ? '○' : '';
      button.className = mark.toLowerCase();
      button.setAttribute('aria-label', `Cell ${index + 1}${mark ? `, ${mark}` : ', empty'}`);
      button.disabled = Boolean(mark) || finished;
      button.addEventListener('click', () => play(index));
      boardElement.append(button);
    });
  }

  function play(index) {
    if (finished || cells[index]) return;
    cells[index] = turn;
    const winningLine = lines.find(line => line.every(position => cells[position] === turn));
    if (winningLine) {
      finished = true;
      statusElement.textContent = `${turn} wins!`;
      render();
      winningLine.forEach(position => boardElement.children[position].classList.add('win'));
      return;
    }
    if (cells.every(Boolean)) {
      finished = true;
      statusElement.textContent = "It's a draw! Play again?";
      render();
      return;
    }
    turn = turn === 'X' ? 'O' : 'X';
    statusElement.textContent = `${turn}'s turn`;
    render();
  }

  resetButton.addEventListener('click', () => {
    cells = Array(9).fill('');
    turn = 'X';
    finished = false;
    statusElement.textContent = "X's turn";
    render();
  });

  render();
})();
