// Populate the sidebar
//
// This is a script, and not included directly in the page, to control the total size of the book.
// The TOC contains an entry for each page, so if each page includes a copy of the TOC,
// the total size of the page becomes O(n**2).
class MDBookSidebarScrollbox extends HTMLElement {
    constructor() {
        super();
    }
    connectedCallback() {
        this.innerHTML = '<ol class="chapter"><li class="chapter-item expanded "><a href="ARCH.html"><strong aria-hidden="true">1.</strong> System Architecture</a></li><li class="chapter-item expanded "><a href="SETUP.html"><strong aria-hidden="true">2.</strong> Environment Setup</a></li><li class="chapter-item expanded "><a href="FW_INTERRUPTS.html"><strong aria-hidden="true">3.</strong> Interrupt Processing</a></li><li class="chapter-item expanded "><a href="FW_MEMORY.html"><strong aria-hidden="true">4.</strong> Memory Management</a></li><li class="chapter-item expanded "><a href="BUILD_SYSTEM.html"><strong aria-hidden="true">5.</strong> Build System</a></li><li class="chapter-item expanded "><a href="CLI_STRUCT.html"><strong aria-hidden="true">6.</strong> CLI Structure</a></li><li class="chapter-item expanded "><a href="BRIDGE_SERIAL.html"><strong aria-hidden="true">7.</strong> Python Bridge: Serial</a></li><li class="chapter-item expanded "><a href="BRIDGE_ASYNC.html"><strong aria-hidden="true">8.</strong> Python Bridge: Async</a></li><li class="chapter-item expanded "><a href="GPIO_MAPPING.html"><strong aria-hidden="true">9.</strong> GPIO Mapping</a></li><li class="chapter-item expanded "><a href="GPIO_INTERRUPTS.html"><strong aria-hidden="true">10.</strong> GPIO Interrupts</a></li><li class="chapter-item expanded "><a href="I2C_TIMING.html"><strong aria-hidden="true">11.</strong> I2C Timing</a></li><li class="chapter-item expanded "><a href="I2C_REGISTERS.html"><strong aria-hidden="true">12.</strong> I2C Registers</a></li><li class="chapter-item expanded "><a href="SPI_LOGIC.html"><strong aria-hidden="true">13.</strong> SPI Logic</a></li><li class="chapter-item expanded "><a href="SPI_DMA.html"><strong aria-hidden="true">14.</strong> SPI DMA Transfers</a></li><li class="chapter-item expanded "><a href="UART_FLOW.html"><strong aria-hidden="true">15.</strong> UART Flow Control</a></li><li class="chapter-item expanded "><a href="SENSOR_BASE.html"><strong aria-hidden="true">16.</strong> Abstract Sensor Base</a></li><li class="chapter-item expanded "><a href="MPU_MOTION.html"><strong aria-hidden="true">17.</strong> MPU6050: Motion Fusion</a></li><li class="chapter-item expanded "><a href="MPU_REGISTERS.html"><strong aria-hidden="true">18.</strong> MPU6050: Register Map</a></li><li class="chapter-item expanded "><a href="BMP_PRESSURE.html"><strong aria-hidden="true">19.</strong> BMP280: Pressure Models</a></li><li class="chapter-item expanded "><a href="BMP_CALIBRATION.html"><strong aria-hidden="true">20.</strong> BMP280: Calibration</a></li><li class="chapter-item expanded "><a href="GPS_NMEA.html"><strong aria-hidden="true">21.</strong> GPS: NMEA Generation</a></li><li class="chapter-item expanded "><a href="GPS_TRAJECTORY.html"><strong aria-hidden="true">22.</strong> GPS: Trajectories</a></li><li class="chapter-item expanded "><a href="HIL_SYNC.html"><strong aria-hidden="true">23.</strong> HIL Synchronization</a></li><li class="chapter-item expanded "><a href="DIAGNOSTICS.html"><strong aria-hidden="true">24.</strong> Real-Time Diagnostics</a></li><li class="chapter-item expanded "><a href="LIMITATIONS.html"><strong aria-hidden="true">25.</strong> System Limitations</a></li><li class="chapter-item expanded "><a href="WIRING.html"><strong aria-hidden="true">26.</strong> Wiring Guide</a></li><li class="chapter-item expanded "><a href="TROUBLESHOOTING.html"><strong aria-hidden="true">27.</strong> Troubleshooting</a></li></ol>';
        // Set the current, active page, and reveal it if it's hidden
        let current_page = document.location.href.toString().split("#")[0].split("?")[0];
        if (current_page.endsWith("/")) {
            current_page += "index.html";
        }
        var links = Array.prototype.slice.call(this.querySelectorAll("a"));
        var l = links.length;
        for (var i = 0; i < l; ++i) {
            var link = links[i];
            var href = link.getAttribute("href");
            if (href && !href.startsWith("#") && !/^(?:[a-z+]+:)?\/\//.test(href)) {
                link.href = path_to_root + href;
            }
            // The "index" page is supposed to alias the first chapter in the book.
            if (link.href === current_page || (i === 0 && path_to_root === "" && current_page.endsWith("/index.html"))) {
                link.classList.add("active");
                var parent = link.parentElement;
                if (parent && parent.classList.contains("chapter-item")) {
                    parent.classList.add("expanded");
                }
                while (parent) {
                    if (parent.tagName === "LI" && parent.previousElementSibling) {
                        if (parent.previousElementSibling.classList.contains("chapter-item")) {
                            parent.previousElementSibling.classList.add("expanded");
                        }
                    }
                    parent = parent.parentElement;
                }
            }
        }
        // Track and set sidebar scroll position
        this.addEventListener('click', function(e) {
            if (e.target.tagName === 'A') {
                sessionStorage.setItem('sidebar-scroll', this.scrollTop);
            }
        }, { passive: true });
        var sidebarScrollTop = sessionStorage.getItem('sidebar-scroll');
        sessionStorage.removeItem('sidebar-scroll');
        if (sidebarScrollTop) {
            // preserve sidebar scroll position when navigating via links within sidebar
            this.scrollTop = sidebarScrollTop;
        } else {
            // scroll sidebar to current active section when navigating via "next/previous chapter" buttons
            var activeSection = document.querySelector('#sidebar .active');
            if (activeSection) {
                activeSection.scrollIntoView({ block: 'center' });
            }
        }
        // Toggle buttons
        var sidebarAnchorToggles = document.querySelectorAll('#sidebar a.toggle');
        function toggleSection(ev) {
            ev.currentTarget.parentElement.classList.toggle('expanded');
        }
        Array.from(sidebarAnchorToggles).forEach(function (el) {
            el.addEventListener('click', toggleSection);
        });
    }
}
window.customElements.define("mdbook-sidebar-scrollbox", MDBookSidebarScrollbox);
