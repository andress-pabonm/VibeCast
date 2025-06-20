document.addEventListener('DOMContentLoaded', () => {
    // Tabs functionality
    const tabs = document.querySelectorAll('.tab');
    const tabContents = document.querySelectorAll('.tab-content');
    
    tabs.forEach(tab => {
        tab.addEventListener('click', () => {
            // Remove active class from all tabs and contents
            tabs.forEach(t => t.classList.remove('active'));
            tabContents.forEach(c => c.classList.remove('active'));
            
            // Add active class to clicked tab and corresponding content
            tab.classList.add('active');
            const tabId = tab.getAttribute('data-tab');
            document.getElementById(tabId).classList.add('active');
        });
    });
    
    // Edit profile button
    const editProfileBtn = document.querySelector('.edit-profile-btn');
    if (editProfileBtn) {
        editProfileBtn.addEventListener('click', () => {
            alert('Funcionalidad de editar perfil se implementará pronto!');
        });
    }
    
    // Table row actions
    const tableRows = document.querySelectorAll('.content-table tbody tr');
    tableRows.forEach(row => {
        row.addEventListener('click', (e) => {
            if (e.target.tagName !== 'I' && e.target.tagName !== 'BUTTON') {
                alert('Reproduciendo canción: ' + row.cells[1].textContent);
            }
        });
    });
});