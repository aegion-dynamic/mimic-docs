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
        this.innerHTML = '<ol class="chapter"><li class="chapter-item expanded "><a href="index.html"><strong aria-hidden="true">1.</strong> Introduction</a></li><li class="chapter-item expanded "><a href="WIRING.html"><strong aria-hidden="true">2.</strong> Wiring Guide</a></li><li class="chapter-item expanded "><a href="SETUP.html"><strong aria-hidden="true">3.</strong> Getting Started &amp; Requirements</a></li><li class="chapter-item expanded "><a href="ARCH.html"><strong aria-hidden="true">4.</strong> System Architecture</a></li><li class="chapter-item expanded affix "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">MIMIC Firmware</li><li class="chapter-item expanded "><a href="FW_OVERVIEW.html"><strong aria-hidden="true">5.</strong> Firmware Overview</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="GPIO_MAPPING.html"><strong aria-hidden="true">5.1.</strong> GPIO Mapping</a></li><li class="chapter-item expanded "><a href="FW_INTERRUPTS.html"><strong aria-hidden="true">5.2.</strong> Interrupt Processing</a></li><li class="chapter-item expanded "><a href="FW_MEMORY.html"><strong aria-hidden="true">5.3.</strong> Memory Management</a></li><li class="chapter-item expanded "><a href="BUILD_SYSTEM.html"><strong aria-hidden="true">5.4.</strong> Build System</a></li><li class="chapter-item expanded "><a href="PERIPHERALS.html"><strong aria-hidden="true">5.5.</strong> Peripheral Control</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="I2C_TIMING.html"><strong aria-hidden="true">5.5.1.</strong> I2C Timing &amp; Registers</a></li><li class="chapter-item expanded "><a href="SPI_LOGIC.html"><strong aria-hidden="true">5.5.2.</strong> SPI Logic &amp; DMA</a></li><li class="chapter-item expanded "><a href="UART_FLOW.html"><strong aria-hidden="true">5.5.3.</strong> UART Data Flow</a></li></ol></li></ol></li><li class="chapter-item expanded "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">MIMIC Bridge</li><li class="chapter-item expanded "><a href="CLI_STRUCT.html"><strong aria-hidden="true">6.</strong> Bridge Overview &amp; CLI</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="BRIDGE_SERIAL.html"><strong aria-hidden="true">6.1.</strong> Python Bridge: Serial</a></li><li class="chapter-item expanded "><a href="BRIDGE_ASYNC.html"><strong aria-hidden="true">6.2.</strong> Python Bridge: Async</a></li><li class="chapter-item expanded "><a href="HIL_SYNC.html"><strong aria-hidden="true">6.3.</strong> HIL Synchronization</a></li><li class="chapter-item expanded "><a href="DIAGNOSTICS.html"><strong aria-hidden="true">6.4.</strong> Real-Time Diagnostics</a></li></ol></li><li class="chapter-item expanded "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">MIMIC Sensors &amp; Contribution</li><li class="chapter-item expanded "><a href="SENSORS_OVERVIEW.html"><strong aria-hidden="true">7.</strong> Sensors Overview</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="SENSOR_BASE.html"><strong aria-hidden="true">7.1.</strong> Abstract Sensor Base</a></li><li class="chapter-item expanded "><a href="MPU_MOTION.html"><strong aria-hidden="true">7.2.</strong> MPU6050 Motion Sensor</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="MPU_REGISTERS.html"><strong aria-hidden="true">7.2.1.</strong> MPU6050 Registers</a></li></ol></li><li class="chapter-item expanded "><a href="BMP_PRESSURE.html"><strong aria-hidden="true">7.3.</strong> BMP280 Environment Sensor</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="BMP_CALIBRATION.html"><strong aria-hidden="true">7.3.1.</strong> BMP280 Calibration</a></li></ol></li><li class="chapter-item expanded "><a href="GPS_NMEA.html"><strong aria-hidden="true">7.4.</strong> GPS Module</a></li><li><ol class="section"><li class="chapter-item expanded "><a href="GPS_TRAJECTORY.html"><strong aria-hidden="true">7.4.1.</strong> GPS Trajectory</a></li></ol></li></ol></li><li class="chapter-item expanded "><li class="spacer"></li><li class="chapter-item expanded affix "><li class="part-title">Appendix</li><li class="chapter-item expanded "><a href="TROUBLESHOOTING.html"><strong aria-hidden="true">8.</strong> Troubleshooting</a></li><li class="chapter-item expanded "><a href="LIMITATIONS.html"><strong aria-hidden="true">9.</strong> System Limitations</a></li></ol>';
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
