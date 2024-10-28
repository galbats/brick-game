document.querySelectorAll('.game-btn').forEach(button => {
    button.addEventListener('click', () => {
        const gameId = button.getAttribute('data-game-id');
        fetch(`/api/games/${gameId}`, { method: 'POST' })
            .then(response => {
                if (!response.ok) {
                    throw new Error('Failed to start the game');
                }
                return response.json();
            })
            .then(data => {
                console.log(`Game started: ${data.message}`);
                if (gameId === '1') {
                    window.location.href = 'race.html';  // Перенаправляем на страницу гонок
                } else if (gameId === '2') {
                    window.location.href = 'tetris.html';  // Перенаправляем на страницу тетриса
                }
            })
            .catch(error => console.error('Error starting the game:', error));
    });
});
